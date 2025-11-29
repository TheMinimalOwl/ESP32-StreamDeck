#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <lvgl.h>

// TFT_eSPI maneja pantalla Y touch
TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  
  // Inicializar pantalla en modo LANDSCAPE
  tft.init();
  tft.setRotation(1); // Landscape (480x320)
  tft.fillScreen(TFT_BLACK);
  
  // Encender retroiluminación
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH);
  
  // Inicializar LVGL
  lv_init();
  
  // Configurar display LVGL
  static lv_disp_draw_buf_t draw_buf;
  static lv_color_t buf[480 * 20];
  
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, 480 * 20);
  
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 480;  // Landscape: 480 ancho
  disp_drv.ver_res = 320;  // Landscape: 320 alto
  disp_drv.flush_cb = [](lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t*)color_p, w * h, true);
    tft.endWrite();
    
    lv_disp_flush_ready(disp_drv);
  };
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);
  
  // Configurar touch
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = [](lv_indev_drv_t * drv, lv_indev_data_t * data) {
    uint16_t touchX, touchY;
    bool touched = tft.getTouch(&touchX, &touchY);
    
    if (touched) {
      data->point.x = touchX;
      data->point.y = touchY;
      data->state = LV_INDEV_STATE_PR;
    } else {
      data->state = LV_INDEV_STATE_REL;
    }
  };
  lv_indev_drv_register(&indev_drv);
  
  // Crear interfaz Stream Deck landscape
  crearInterfazStreamDeckLandscape();
  
  Serial.println("✅ Stream Deck Landscape 2x4 inicializado");
}

void loop() {
  lv_timer_handler();
  delay(5);
}

void crearInterfazStreamDeckLandscape() {
  // Limpiar pantalla
  lv_obj_clean(lv_scr_act());
  
  // Configuración para 8 botones en 2 filas × 4 columnas
  const uint8_t filas = 2;
  const uint8_t columnas = 4;
  const uint16_t anchoBoton = 90;   // Botones cuadrados
  const uint16_t altoBoton = 90;    // Botones cuadrados
  const uint16_t espacioX = 25;     // Espacio horizontal entre botones
  const uint16_t espacioY = 40;     // Espacio vertical entre filas
  
  // Calcular posición inicial para centrar los botones
  const uint16_t totalAncho = (columnas * anchoBoton) + ((columnas - 1) * espacioX);
  const uint16_t totalAlto = (filas * altoBoton) + ((filas - 1) * espacioY);
  const uint16_t startX = (480 - totalAncho) / 2;  // Centrar horizontalmente
  const uint16_t startY = (320 - totalAlto) / 2;   // Centrar verticalmente
  
  // Textos para los botones (personalizables)
  const char* textosBotones[8] = {
    "F1", "F2", "F3", "F4",
    "F5", "F6", "F7", "F8"
  };
  
  // Colores para los botones (opcional)
  lv_color_t coloresBotones[8] = {
    lv_color_hex(0x264653), lv_color_hex(0x2a9d8f), lv_color_hex(0xe9c46a), lv_color_hex(0xf4a261),
    lv_color_hex(0xe76f51), lv_color_hex(0x3a86ff), lv_color_hex(0x8338ec), lv_color_hex(0xff006e)
  };
  
  // Crear los 8 botones
  for (uint8_t fila = 0; fila < filas; fila++) {
    for (uint8_t col = 0; col < columnas; col++) {
      uint8_t indice = fila * columnas + col;
      
      // Calcular posición
      uint16_t x = startX + col * (anchoBoton + espacioX);
      uint16_t y = startY + fila * (altoBoton + espacioY);
      
      // Crear botón cuadrado
      lv_obj_t *btn = lv_btn_create(lv_scr_act());
      lv_obj_set_size(btn, anchoBoton, altoBoton);
      lv_obj_set_pos(btn, x, y);
      
      // Estilo del botón - COLORES DISTINTOS
      lv_obj_set_style_bg_color(btn, coloresBotones[indice], 0);
      lv_obj_set_style_bg_color(btn, lv_color_lighten(coloresBotones[indice], 50), LV_STATE_PRESSED);
      lv_obj_set_style_radius(btn, 12, 0);
      lv_obj_set_style_border_width(btn, 2, 0);
      lv_obj_set_style_border_color(btn, lv_color_white(), 0);
      
      // Etiqueta del botón
      lv_obj_t *label = lv_label_create(btn);
      lv_label_set_text(label, textosBotones[indice]);
      lv_obj_center(label);
      lv_obj_set_style_text_color(label, lv_color_white(), 0);
      lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0);
      
      // Evento de click - CON FEEDBACK VISUAL
      lv_obj_add_event_cb(btn, [](lv_event_t * e) {
        lv_obj_t *btn = lv_event_get_target(e);
        lv_obj_t *label = lv_obj_get_child(btn, 0);
        const char *texto = lv_label_get_text(label);
        
        // Feedback visual temporal
        lv_obj_set_style_bg_color(btn, lv_color_white(), LV_STATE_PRESSED);
        Serial.printf("🎮 Botón presionado: %s\n", texto);
        
        // Aquí añadirás la lógica de cada botón
        // Por ejemplo: enviar tecla, controlar OBS, etc.
        
      }, LV_EVENT_CLICKED, NULL);
    }
  }
  
  // Título superior
  lv_obj_t *titulo = lv_label_create(lv_scr_act());
  lv_label_set_text(titulo, "ESP32 STREAM DECK");
  lv_obj_set_style_text_font(titulo, &lv_font_montserrat_14, 0);
  lv_obj_set_style_text_color(titulo, lv_color_hex(0xFFFFFF), 0);
  lv_obj_align(titulo, LV_ALIGN_TOP_MID, 0, 15);
  
  // Footer con información
  lv_obj_t *footer = lv_label_create(lv_scr_act());
  lv_label_set_text(footer, "8 Botones • Landscape • 480x320");
  lv_obj_set_style_text_font(footer, &lv_font_montserrat_14, 0);
  lv_obj_set_style_text_color(footer, lv_color_hex(0xCCCCCC), 0);
  lv_obj_align(footer, LV_ALIGN_BOTTOM_MID, 0, -10);
}