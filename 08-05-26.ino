#include "ESP_Panel_Library.h"
#include "lvgl_v8_port.h"
#include "ui.h"
#include <esp_heap_caps.h>
#include <esp_partition.h>
#include <WiFi.h>

ESP_Panel *panel = new ESP_Panel();

// -------------------------------------------------------------------
// Fungsi bantu untuk mencetak informasi memori
// -------------------------------------------------------------------
void print_memory_report(const char *label) {
    size_t free_internal = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
    size_t free_spiram  = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    size_t free_dma     = heap_caps_get_free_size(MALLOC_CAP_DMA);

    Serial.printf("=== %s ===\n", label);
    Serial.printf("Free internal heap : %6u bytes (%u KB)\n",
                  free_internal, free_internal / 1024);
    Serial.printf("Free PSRAM         : %6u bytes (%u KB)\n",
                  free_spiram, free_spiram / 1024);
    Serial.printf("Free DMA-capable   : %6u bytes (%u KB)\n",
                  free_dma, free_dma / 1024);
    Serial.println();
}

// -------------------------------------------------------------------
void setup() {
    Serial.begin(115200);
    delay(1000);

    // --- Sebelum WiFi mati ---
    print_memory_report("Sebelum WiFi mati");

    // Matikan WiFi
    WiFi.mode(WIFI_OFF);
    Serial.println("WiFi telah dimatikan.");

    // --- Setelah WiFi mati ---
    print_memory_report("Setelah WiFi mati");

    // Inisialisasi board & LVGL
    panel->init();
    panel->begin();
    lvgl_port_init(panel->getLcd(), panel->getTouch());

    print_memory_report("Setelah board & LVGL siap");

    // Jalankan UI
    ui_init();
    print_memory_report("Setelah UI_init()");

    Serial.println("UI dimulai.");
}

void loop() {
    lvgl_port_lock(0);
    lv_task_handler();
    lvgl_port_unlock();
    delay(5);
}