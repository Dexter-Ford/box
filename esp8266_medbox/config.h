#pragma once
/*
 * config.h — ค่าตั้งต้นทั้งหมดของกล่องยา
 * แก้ไฟล์นี้ไฟล์เดียว ไม่ต้องไปแก้ใน .ino
 */

// ===================== WiFi =====================
#define WIFI_SSID       "tintin_2.4GHz"
#define WIFI_PASSWORD   "tintin123"
#define WIFI_HOSTNAME   "medbox-01"
#define WIFI_TIMEOUT_MS 20000UL

// ===================== เวลา =====================
#define TZ_OFFSET_SEC   (7 * 3600)          // ไทย UTC+7
#define NTP_SERVER1     "pool.ntp.org"
#define NTP_SERVER2     "time.nist.gov"
#define NTP_RESYNC_MS   (6UL * 60 * 60 * 1000)  // ซิงค์ NTP ซ้ำทุก 6 ชม. กัน RTC เพี้ยน
#define RTC_MIN_YEAR    2024                // อ่าน RTC ได้ปีต่ำกว่านี้ = ถือว่าเวลายังไม่ถูก (ถ่านหมด/ยังไม่ตั้ง)

// ===================== ความลับ (ไม่เข้า git) =====================
// token / chat id / URL จริงเก็บไว้ใน secrets.h (ดู secrets.h.example)
// ถ้าไม่มีไฟล์ secrets.h ระบบยังทำงานได้ แต่จะไม่ส่ง Telegram/ไม่ยิง backend
#if __has_include("secrets.h")
#include "secrets.h"
#endif

// ===================== Telegram =====================
// ว่างไว้ = ปิดการแจ้งเตือน (ระบบยังทำงานบน OLED/บัสเซอร์ได้)
#ifndef TELEGRAM_BOT_TOKEN
#define TELEGRAM_BOT_TOKEN ""   // จาก @BotFather (ใส่ใน secrets.h)
#endif
#ifndef TELEGRAM_CHAT_ID
#define TELEGRAM_CHAT_ID   ""   // chat id ของผู้สูงอายุ/ผู้ดูแล (ใส่ใน secrets.h)
#endif

// ===================== Backend / Camera =====================
#ifndef BACKEND_URL
#define BACKEND_URL       ""    // เช่น "http://192.168.1.10:3000/api/events" (ว่าง = ปิด)
#endif
#ifndef CAM_NODE_URL
#define CAM_NODE_URL      ""    // เช่น "http://192.168.1.61/verify"     (ว่าง = ข้ามการสแกนหน้า)
#endif
#define FACE_VERIFY_ENABLED true

// ===================== ขา (NodeMCU v2) =====================
// หมายเหตุ: ค่าต่อไปนี้ยืนยันจาก self-test จริงบนบอร์ด (0x3C/0x57/0x68 ตอบสนอง)
#define PIN_I2C_SDA     D1   // GPIO5  <-- SDA
#define PIN_I2C_SCL     D2   // GPIO4  <-- SCL
#define PIN_HX711_DT    D5   // GPIO14 <-- ยืนยันจาก self-test รอบ 2 (DT ต่อ D5)
#define PIN_HX711_SCK   D6   // GPIO12 <-- SCK ต่อ D6 (สลับจากค่าตั้งต้นเดิม)
#define PIN_IR_SENSOR   A0   // <-- IR sensor (OUT ของโมดูล) ย้ายจาก D7/GPIO13 มา A0
                             //   เหตุผล: A0 เป็น ADC ไม่ใช่ขา strapping -> บูต/แฟลชได้เสมอ
                             //   แม้สาย OUT ค้าง LOW/HIGH ก็ไม่ทำให้บอร์ดบูตไม่ขึ้น
                             //   จะกลับไปใช้ D7: ตั้ง PIN_IR_SENSOR D7 + IR_USE_ANALOG 0
#define IR_USE_ANALOG   1    // 1 = อ่านด้วย analogRead (ต้องใช้กับ A0) · 0 = digitalRead (ใช้กับ D7)
#define IR_ANALOG_THRESHOLD 512  // (ใช้เมื่อ IR_ANALOG_AUTO_BASELINE false) analogRead > นี้ = HIGH (ว่าง)
                                 //   ★ ปกติไม่ต้องใช้ค่านี้แล้ว ปล่อยให้ auto-baseline วัดเองด้านล่าง
#define PIN_BUZZER      D8   // GPIO15 <-- ย้ายมาจาก D7 เพราะ D7 ให้ IR
                             //   GPIO15 ต้อง LOW ตอนบูต ซึ่งตรงกับ buzzer ที่ยังไม่ทำงาน = ปลอดภัย
                             //   ถ้าอยากคง buzzer ที่ D7 ให้สลับเป็น: PIN_IR_SENSOR D8 / PIN_BUZZER D7
                             //   (ทำได้เฉพาะเมื่อโมดูล IR ปล่อย LOW ตอนไม่มีคน ไม่งั้นบอร์ดจะบูตไม่ขึ้น)
#define PIN_LED         D4   // GPIO2
#define PIN_BTN_TAKEN   D0   // GPIO16  -> ต้องมี R pull-up 10k ภายนอก
#define PIN_BTN_SNOOZE  D3   // GPIO0   -> ใช้ INPUT_PULLUP ภายในได้

// ===================== IR sensor =====================
// โมดูล IR แบบที่ใช้ในตัวอย่าง (IR_Sensor_example1) : ไม่มีคน = HIGH, มีคน = LOW
#define IR_ACTIVE_LOW        true    // true = ตรวจพบเมื่ออ่านได้ LOW
#define IR_CONFIRM_MS        120     // กันกระพริบ/เสียงรบกวน

// ★★ วิธีตรวจ "มีคน" บนขา analog (A0) — 2 โหมด
//   โหมด A (แนะนำ, ค่าเริ่มต้น): auto-baseline — ตอนบูตจำค่า A0 ตอน "ยังไม่มีคน" ไว้ แล้วเทียบว่าค่า
//           เปลี่ยนไปจากเดิมมากน้อยแค่ไหน (ไม่ต้องรู้ค่า absolute, ไม่สนว่าค่าจะลดหรือเพิ่มตอนมีคน)
//   โหมด B (เดิม): เทียบกับ IR_ANALOG_THRESHOLD ตายตัว ± IR_ANALOG_HYST — ใช้เมื่อรู้ค่าจริงแล้วและค่านิ่ง
#define IR_ANALOG_AUTO_BASELINE true // ★ true = วัด baseline เองตอนบูต (ทนกว่า ใช้ได้กับทุกโมดูล)
                                     //   ข้อควรระวัง: 0.5 วิแรกตอนเปิดเครื่องอย่าเอามือ/ของบังหน้าเซนเซอร์
#define IR_ANALOG_DELTA      150     // (โหมด A) ค่า A0 ต่างจาก baseline เกินกี่หน่วย = "มีคน"
                                     //   ดูค่าจริงได้จาก log `[IR] A0=.. b=.. d=..` (เปิดด้วย IR_DEBUG_LOG)
                                     //   ถ้าบังมือแล้ว d ขึ้นแค่ ~40-80 ให้ลดค่านี้ (เช่น 40) · ถ้า noise เยอะให้เพิ่ม
#define IR_ANALOG_EXIT_PCT   50      // (โหมด A) ออกจาก "มีคน" เมื่อ diff ต่ำกว่า DELTA x 50% (กันกระพริบ)
#define IR_ANALOG_HYST       120     // (โหมด B เท่านั้น) ± รอบ threshold กันค่าแกว่งแล้วสถานะสลับไปมา
                                     //   ตอนไม่มีคนต้องสว่างกว่า 512+120 / ตอนมีคนต้องมืดกว่า 512-120 กว่าจะสลับ
                                     //   ถ้าเซนเซอร์ยังสั่นอยู่ ให้เพิ่มเป็น 200 (หรือปรับ trimpot ให้ idle ≈ 1024, มือบัง ≈ 10)
#define IR_DEBUG_LOG         true    // พิมพ์ `[IR] A0=.. b=.. d=..` ทุก 1 วิ (เปิดตอนจูน/แก้ปัญหา, ปิดตอนใช้งานจริง)

#define IR_REARM_MS          5000UL  // (ใช้เมื่อ IR_SILENCE_LATCH = false) คนเดินออกแล้วเงียบเกิน 5 วิ -> กลับมาเตือนใหม่
#define IR_SILENCE_LATCH     true    // ★ true = บังมือแล้ว "ปิดเสียงค้าง" จนกว่าจะกด "รับยา"/"snooze" หรือครบเวลา
                                     //   ถอยมือออกแล้วเสียงไม่กลับมาเอง (พฤติกรรมที่ต้องการ: บังแล้วหยุดเลย)
                                     //   false = พฤติกรรมเดิม: ถอยมือ 5 วิ แล้วกลับมาเตือน (ดู IR_REARM_MS)
#define IR_LATCH_MAX_MS      (5UL * 60 * 1000UL)  // กันลืม: เงียบค้างเกิน 5 นาทีโดยไม่มีใครกดรับยา -> เตือนกลับมา
                                     //   ตั้ง 0 = เงียบไปจนครบ RESPONSE_TIMEOUT_MS (15 นาที) แล้วขึ้น "ลืมกินยา"
#define IR_STUCK_MS          120000UL // อ่าน "มีคน" ค้างเกิน 2 นาที (ตอนไม่มีการเตือน) = เซนเซอร์ผิดปกติ -> ปิดใช้ IR
#define IR_ENABLED           true

// ===================== ตารางยา =====================
// ★ แก้ตารางได้ 2 ทาง
//   1) แก้ DOSE_SCHEDULE_DEFAULT ที่นี่ (ค่าเริ่มต้นในเฟิร์มแวร์)
//   2) เปิด http://<ไอพีโน้ตบุ๊ก>:3000/schedule แล้วกดบันทึก -> เครื่องดึงไปใช้เอง
//      (ตอนบูต / ทุก 10 นาที / พิมพ์ "ตาราง" ใน Telegram) ไม่ต้องแฟลชใหม่
#define DOSE_MAX 8                      // จำนวนมื้อสูงสุดที่รับได้
#define LABEL_LEN 20                    // ความยาวชื่อมื้อ (รวม \0) — OLED แสดงได้เฉพาะ ASCII

struct DoseSlot {
  uint8_t hour;
  uint8_t minute;
  char    label[LABEL_LEN];   // เก็บเป็น array (ไม่ใช่ pointer) เพื่อให้คัดลอกจาก backend ได้
};

static const DoseSlot DOSE_SCHEDULE_DEFAULT[] = {
  { 8,  0, "Morning 08:00" },
  { 12, 0, "Noon 12:00" },
  { 18, 0, "Evening 18:00" },
};
#define DOSE_DEFAULT_COUNT (sizeof(DOSE_SCHEDULE_DEFAULT) / sizeof(DoseSlot))

// ที่อยู่ดึงตารางยา: เว้นว่าง = ดึงจาก BACKEND_URL ตัวเดียวกัน (ตัด /api/... ออกแล้วต่อ /api/schedule)
#ifndef SCHEDULE_URL
#define SCHEDULE_URL ""
#endif
#define SCHEDULE_REFRESH_MS (10UL * 60 * 1000)   // ดึงตารางซ้ำทุก 10 นาที

// ===================== ปุ่มกด =====================
// (ประกาศไว้ใน header เพื่อให้ Arduino สร้าง prototype ของฟังก์ชันที่รับ Btn ได้)
struct Btn {
  uint8_t  pin;
  int      last;
  uint32_t t;
};

// ===================== Load cell / HX711 =====================
#define PILLS_INITIAL        30      // จำนวนยาเริ่มต้น (ใช้เมื่อยังไม่คาลิเบรต)
#define LOADCELL_CALIBRATION 0.0f    // ค่าจาก set_scale(); 0 = ใช้จำนวนยาคงที่
#define EMPTY_TRAY_WEIGHT_G  0.0f    // น้ำหนักถาดเปล่า (กรัม)
#define PILL_WEIGHT_G        0.50f   // น้ำหนักยาต่อเม็ด (กรัม)
#define LOW_STOCK_PILLS      5       // แจ้งเตือนเมื่อเหลือน้อยกว่านี้

// ===================== พฤติกรรม / เวลา =====================
// ★ โหมดเดโม: true = ย่นเวลาเพื่อสาธิต (ไม่ต้องรอถึง 08:00)
//   - เตือนซ้ำทุก 30 วิ (ปกติ 3 นาที)
//   - รอตอบ 2 นาที (ปกติ 15 นาที) แล้วถือว่า "ข้ามมื้อ"
//   - Snooze 1 นาที (ปกติ 15 นาที)
//   เปิด/ปิดได้ระหว่างใช้งานด้วยคำสั่ง Telegram "ทดสอบ" (เริ่มเตือนทันที) ไม่ต้องแฟลชใหม่
#define DEMO_MODE            false

#define IDLE_CHECK_MS        30000UL      // ตรวจตารางยาทุก 30 วินาที (30–60 ตาม flowchart)
#define DOSE_WINDOW_MIN      30           // ถ้าเลยเวลาเกิน 30 นาทีถือว่าข้ามมื้อนั้น

#if DEMO_MODE
#define SNOOZE_MINUTES       1
#define REPEAT_REMIND_MS     (30UL * 1000)        // 30 วินาที
#define RESPONSE_TIMEOUT_MS  (2UL * 60 * 1000)    // 2 นาที
#else
#define SNOOZE_MINUTES       15
#define REPEAT_REMIND_MS     (3UL * 60 * 1000)    // เตือนซ้ำทุก 3 นาที
#define RESPONSE_TIMEOUT_MS  (15UL * 60 * 1000)   // ไม่ตอบ 15 นาที = ลืมกิน
#endif
#define MAX_FACE_TRIES       3
#define DAILY_SUMMARY_HOUR   21
#define BUZZER_FREQ_HZ       2300
// ขั้วของบัซเซอร์ที่ D8 (★ ยืนยันจากของจริงบนกล่องนี้ 22 พ.ย. 2569 — ค่าที่ใช้จริงคือ 1):
//   0 = ดังเมื่อขา HIGH (บัซเซอร์ 2 ขาแบบ passive / โมดูล active-HIGH)
//       -> ตอนเงียบใช้ noTone() (ESP8266 core จะ digitalWrite(pin, LOW) ให้เอง)
//   1 = โมดูล trigger ด้วย LOW (I-O = LOW แล้วดัง) -> ตอนเงียบต้อง "ขับ D8 เป็น HIGH"
//
// หลักฐานที่สรุปว่าเป็นแบบ 1 (active-LOW):
//   - สั่ง "เงียบ" แล้วได้ยินเสียงยาวต่อเนื่อง (noTone() ดึงขาลง LOW -> โมดูลดัง)
//   - สั่ง "เตือน" ด้วย tone() ขาจะสลับ HIGH/LOW -> ได้ยิน "เสียงเปลี่ยน/กระตุก" ไม่ใช่เสียงสนั่น
//   - firmware/04_buzzer_test บูตมาเงียบ (มันขับ HIGH ตอน idle) = ตรงกับนิยาม active-LOW
// โหมด 1 จะ "ขับระดับตรงๆ" (LOW=ดัง, HIGH=เงียบ) ไม่เรียก tone() เพราะโมดูลมีออสซิลเลเตอร์ในตัว
#define BUZZER_ACTIVE_LOW    1
#define BUZZER_DEBUG_LOG     true   // พิมพ์ `[BUZZ] ON -> D8=LOW` ทุกครั้งที่เปลี่ยนสถานะ
                                     //   ★ ใช้ตรวจการแก้บั๊ก "บังมือแล้วเสียงไม่ดับ" ได้โดยไม่ต้องต่อบัซเซอร์:
                                     //     เห็น `[BUZZ] OFF -> D8=HIGH` = firmware สั่งเงียบจริง (ปิดตอนส่งงานได้)
#define LED_BLINK_MS         400

// ===================== ตัวเลือกขั้นสูง =====================
#define DEEP_SLEEP_ENABLED   false   // true = ประหยัดไฟสุด แต่ปุ่มจะไม่ทำงานตอนหลับ
#define ENABLE_BATTERY_CHECK false   // true = ต้องมี divider ที่ A0
#define BATTERY_ADC_PIN      A0
#define BATTERY_LOW_VOLT     3.40f
#define BATTERY_DIVIDER      2.0f    // 100k/100k -> 2.0
