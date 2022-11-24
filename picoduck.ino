#include "common.h"
#include "lua_api.h"

#include "SPI.h"
#include "SdFat.h"

#include "Adafruit_SPIFlash.h"

#include "ff.h"
#include "diskio.h"

#define DISK_LABEL "Picoduck"

Adafruit_FlashTransport_RP2040 flashTransport;

bool usb_mass_storage = false;

Adafruit_SPIFlash flash(&flashTransport);

void format_fat12(void) {
  // Working buffer for f_mkfs.
  uint8_t workbuf[4096];

  // Elm Cham's fatfs objects
  FATFS elmchamFatfs;

  // Make filesystem.
  FRESULT r = f_mkfs("", FM_FAT, 0, workbuf, sizeof(workbuf));
  if (r != FR_OK) {
    Serial.print(F("Error, f_mkfs failed with error code: ")); Serial.println(r, DEC);
    while(1) yield();
  }

  // mount to set disk label
  r = f_mount(&elmchamFatfs, "0:", 1);
  if (r != FR_OK) {
    Serial.print(F("Error, f_mount failed with error code: ")); Serial.println(r, DEC);
    while(1) yield();
  }

  // Setting label
  Serial.println(F("Setting disk label to: " DISK_LABEL));
  r = f_setlabel(DISK_LABEL);
  if (r != FR_OK) {
    Serial.print(F("Error, f_setlabel failed with error code: ")); Serial.println(r, DEC);
    while(1) yield();
  }

  // unmount
  f_unmount("0:");

  // sync to make sure all data is written to flash
  flash.syncBlocks();

  Serial.println(F("Formatted flash!"));
}

// file system object from SdFat
FatVolume fatfs;

Adafruit_USBD_MSC usb_msc;

// Check if flash is formatted
bool fs_formatted;

byte get_program_index_from_pins() {
  byte final = 0;
  if (digitalRead(PIN_B0) == HIGH) final += 1;
  if (digitalRead(PIN_B1) == HIGH) final += 2;
  if (digitalRead(PIN_B2) == HIGH) final += 4;
  if (digitalRead(PIN_B3) == HIGH) final += 8;
  return final;
}

int32_t msc_read_cb (uint32_t lba, void* buffer, uint32_t bufsize) {
  return flash.readBlocks(lba, (uint8_t*) buffer, bufsize/512) ? bufsize : -1;
}

int32_t msc_write_cb (uint32_t lba, uint8_t* buffer, uint32_t bufsize) {
  digitalWrite(LED_BUILTIN, HIGH);
  return flash.writeBlocks(lba, buffer, bufsize/512) ? bufsize : -1;
}

void msc_flush_cb (void) {
  flash.syncBlocks();
  fatfs.cacheClear();
  digitalWrite(LED_BUILTIN, LOW);
}

void led_loop(int rate=1000) {
  for (;;) {
    digitalWrite(BUILT_IN_LED_PIN, HIGH);
    delay(rate);
    digitalWrite(BUILT_IN_LED_PIN, LOW);
    delay(rate);
  }
}

void setup() {
  Serial.begin(9600);
  
  pinMode(BUILT_IN_LED_PIN, OUTPUT);
  pinMode(EXTERNAL_LED_PIN, OUTPUT);

  // Random seed from ADC noise.
  pinMode(26, INPUT);
  srand(analogRead(26) * 10000);

  pinMode(HOLD_PIN, INPUT);
  pinMode(MSD_PIN, INPUT);

  pinMode(PIN_B0, INPUT);
  pinMode(PIN_B1, INPUT);
  pinMode(PIN_B2, INPUT);
  pinMode(PIN_B3, INPUT);

  byte program_index = get_program_index_from_pins();
#ifdef DONT_USE_MSD_PIN
  if (program_index == 0b1111) {
#else
  if (digitalRead(MSD_PIN) == HIGH) {
#endif
    usb_mass_storage = true;
  } else {
    Keyboard.begin();
    Mouse.begin();
  }

  flash.begin();

  if (usb_mass_storage) {
    usb_msc.setID("Picoduck", "Internal Memory", "1.0");

    usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);

    usb_msc.setCapacity(flash.size() / 512, 512);

    usb_msc.setUnitReady(true);
    usb_msc.begin();
  }

  fs_formatted = fatfs.begin(&flash);
  if (!fs_formatted) {
    format_fat12();
    
    fs_formatted = fatfs.begin(&flash);
    if (!fs_formatted) {
      led_loop();
    }
  }

  if (usb_mass_storage) {
    digitalWrite(BUILT_IN_LED_PIN, HIGH);
    return;
  }

  String program_code_path = String(program_index) + ".lua";

  File32 file = fatfs.open(program_code_path.c_str(), FILE_READ);
  if (!file)
    led_loop(500);

  char file_contents[file.available()+1] = { '\0' };
  file.read(file_contents, file.available());
  file.close();

  fatfs.end();
  flash.end();

  lua_State *L = luaL_newstate();
  if (L == 0)
    return;

  luaL_openlibs(L);

  lua_settop(L, 0);

  l_init(L);

  luaL_loadstring(L, file_contents);
  
  if (lua_pcall(L, 0, 0, 0) != 0) {
    Serial.print("Lua error: ");
    Serial.println(lua_tostring(L,-1));
  } else {
    Serial.println("Program ran successfully.");
  }
}

void loop() { }

void setup1() { }

void loop1() {
  hold = digitalRead(HOLD_PIN) == HIGH;
}

// fatfs diskio
extern "C" {

DSTATUS disk_status ( BYTE pdrv ) {
  (void) pdrv;
  return 0;
}

DSTATUS disk_initialize ( BYTE pdrv ) {
  (void) pdrv;
  return 0;
}

DRESULT disk_read (
  BYTE pdrv,    // Physical drive nmuber to identify the drive
  BYTE *buff,   // Data buffer to store read data
  DWORD sector, // Start sector in LBA
  UINT count    // Number of sectors to read
) {
  (void) pdrv;
  return flash.readBlocks(sector, buff, count) ? RES_OK : RES_ERROR;
}

DRESULT disk_write (
  BYTE pdrv,        // Physical drive nmuber to identify the drive
  const BYTE *buff, // Data to be written
  DWORD sector,     // Start sector in LBA
  UINT count        // Number of sectors to write
){
  (void) pdrv;
  return flash.writeBlocks(sector, buff, count) ? RES_OK : RES_ERROR;
}

DRESULT disk_ioctl (
  BYTE pdrv,  // Physical drive nmuber (0..)
  BYTE cmd,   // Control code
  void *buff  // Buffer to send/receive control data
) {
  (void) pdrv;

  switch ( cmd )
  {
    case CTRL_SYNC:
      flash.syncBlocks();
      return RES_OK;

    case GET_SECTOR_COUNT:
      *((DWORD*) buff) = flash.size()/512;
      return RES_OK;

    case GET_SECTOR_SIZE:
      *((WORD*) buff) = 512;
      return RES_OK;

    case GET_BLOCK_SIZE:
      *((DWORD*) buff) = 8;    // erase block size in units of sector size
      return RES_OK;

    default:
      return RES_PARERR;
  }
}

}
