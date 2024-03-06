#include "secret.h"
#include <string.h>
#include "common.h"
#include "flash.h"
#include "model.h"

static secbool bootloader_locked_set = secfalse;
static secbool bootloader_locked = secfalse;

secbool secret_verify_header(void) {
  uint8_t header[sizeof(SECRET_HEADER_MAGIC)] = {0};

  memcpy(header,
         flash_area_get_address(&SECRET_AREA, 0, sizeof(SECRET_HEADER_MAGIC)),
         sizeof(SECRET_HEADER_MAGIC));

  bootloader_locked =
      memcmp(header, SECRET_HEADER_MAGIC, sizeof(SECRET_HEADER_MAGIC)) == 0
          ? sectrue
          : secfalse;
  bootloader_locked_set = sectrue;
  return bootloader_locked;
}

secbool secret_bootloader_locked(void) {
  if (bootloader_locked_set != sectrue) {
    // Set bootloader_locked.
    secret_verify_header();
  }

  return bootloader_locked;
}

void secret_write_header(void) {
  uint8_t header[SECRET_HEADER_LEN] = {0};
  memcpy(header, SECRET_HEADER_MAGIC, 4);
  secret_write(header, 0, SECRET_HEADER_LEN);
}

void secret_write(const uint8_t* data, uint32_t offset, uint32_t len) {
  ensure(flash_unlock_write(), "secret write");
  for (int i = 0; i < len; i++) {
    ensure(flash_area_write_byte(&SECRET_AREA, offset + i, data[i]),
           "secret write");
  }
  ensure(flash_lock_write(), "secret write");
}

secbool secret_read(uint8_t* data, uint32_t offset, uint32_t len) {
  if (sectrue != secret_verify_header()) {
    return secfalse;
  }

  memcpy(data, flash_area_get_address(&SECRET_AREA, offset, len), len);

  return sectrue;
}

secbool secret_wiped(void) {
  flash_area_get_address(&SECRET_AREA, 0, 1);

  flash_area_get_size(&SECRET_AREA);

  uint32_t size = flash_area_get_size(&SECRET_AREA);

  for (int i = 0; i < size; i += 4) {
    uint32_t* addr = (uint32_t*)flash_area_get_address(&SECRET_AREA, i, 4);
    if (*addr != 0xFFFFFFFF) {
      return secfalse;
    }
  }
  return sectrue;
}

void secret_erase(void) {
  ensure(flash_area_erase(&SECRET_AREA, NULL), "secret erase");
}

secbool secret_optiga_set(const uint8_t secret[SECRET_OPTIGA_KEY_LEN]) {
  secret_erase();
  secret_write_header();
  secret_write(secret, SECRET_OPTIGA_KEY_OFFSET, SECRET_OPTIGA_KEY_LEN);
  return sectrue;
}

secbool secret_optiga_get(uint8_t dest[SECRET_OPTIGA_KEY_LEN]) {
  return secret_read(dest, SECRET_OPTIGA_KEY_OFFSET, SECRET_OPTIGA_KEY_LEN);
}
