#include "harmonograph.h"
#include "util/util_math.h"
#include "OC_digital_inputs.h"

enum HARMONOGRAPH_SETTINGS {
  HARMONOGRAPH_SETTING_FREQ,
  HARMONOGRAPH_SETTING_LAST
};


class HarmonoGraph : public settings::SettingsBase<HarmonoGraph, HARMONOGRAPH_SETTING_LAST> {
public:

  uint16_t get_freq() const {
    return values_[HARMONOGRAPH_SETTING_FREQ];
  }


  void Init();

  void freeze() {
    frozen_ = true;
  }

  void thaw() {
    frozen_ = false;
  }

  bool frozen() const {
    return frozen_;
  }

  streams::HarmonoGraph harmonograph;
  bool frozen_;

  // ISR update is at 16.666kHz, we don't need it that fast so smooth the values to ~1Khz
  static constexpr int32_t kSmoothing = 16;

  SmoothedValue<int32_t, kSmoothing> cv_freq;
  // SmoothedValue<int32_t, kSmoothing> cv_freq2;
  // SmoothedValue<int32_t, kSmoothing> cv_rho1;
  // SmoothedValue<int32_t, kSmoothing> cv_rho2;
};

void HarmonoGraph::Init() {
  InitDefaults();
  harmonograph.Init();
  frozen_= false;
}

SETTINGS_DECLARE(HarmonoGraph, HARMONOGRAPH_SETTING_LAST) {
  { 0, 0, 255, "FREQ", NULL, settings::STORAGE_TYPE_U8 },
};

HarmonoGraph harmono_graph;
struct {
  int selected_param;
} harmono_graph_state;


#define SCALE8_16(x) ((((x + 1) << 16) >> 8) - 1)

void FASTRUN HARMONOGRAPH_isr() {

  bool reset_phase = OC::DigitalInputs::clocked<OC::DIGITAL_INPUT_1>();
  bool freeze = OC::DigitalInputs::read_immediate<OC::DIGITAL_INPUT_2>();
  // bool reset_phase2 = OC::DigitalInputs::clocked<OC::DIGITAL_INPUT_3>();
  //bool freeze2 = OC::DigitalInputs::read_immediate<OC::DIGITAL_INPUT_4>();

  harmono_graph.cv_freq.push(OC::ADC::value<ADC_CHANNEL_1>());
  // harmono_graph.cv_rho1.push(OC::ADC::value<ADC_CHANNEL_2>());
  // harmono_graph.cv_freq2.push(OC::ADC::value<ADC_CHANNEL_3>());
  // harmono_graph.cv_rho2.push(OC::ADC::value<ADC_CHANNEL_4>());

  // Range in settings is (0-256] so this gets scaled to (0,65535]
  // CV value is 12 bit so also needs scaling

  int32_t freq = SCALE8_16(harmono_graph.get_freq()) + (harmono_graph.cv_freq.value() * 16);
  freq = USAT16(freq);


  if (!freeze && !harmono_graph.frozen())
    harmono_graph.harmonograph.Process(freq, reset_phase);

  DAC::set<DAC_CHANNEL_A>(harmono_graph.harmonograph.dac_code(0));
  DAC::set<DAC_CHANNEL_B>(harmono_graph.harmonograph.dac_code(1));
  DAC::set<DAC_CHANNEL_C>(harmono_graph.harmonograph.dac_code(2));
  DAC::set<DAC_CHANNEL_D>(harmono_graph.harmonograph.dac_code(3));
}

void HARMONOGRAPH_init() {
  harmono_graph_state.selected_param = HARMONOGRAPH_SETTING_FREQ;
  harmono_graph.Init();
}

// Up to here.

size_t HARMONOGRAPH_storageSize() {
  return HarmonoGraph::storageSize();
}

size_t HARMONOGRAPH_save(void *storage) {
  return harmono_graph.Save(storage);
}

size_t HARMONOGRAPH_restore(const void *storage) {
  return harmono_graph.Restore(storage);
}

void HARMONOGRAPH_loop() {
  if (_ENC && (millis() - _BUTTONS_TIMESTAMP > DEBOUNCE)) encoders();
  buttons(BUTTON_TOP);
  buttons(BUTTON_BOTTOM);
  buttons(BUTTON_LEFT);
  buttons(BUTTON_RIGHT);
}

void HARMONOGRAPH_menu() {
  GRAPHICS_BEGIN_FRAME(false); // no frame, no problem

  graphics.setFont(UI_DEFAULT_FONT);

  static const weegfx::coord_t kStartX = 0;
  UI_DRAW_TITLE(kStartX);
  graphics.setPrintPos(2, 2);
  graphics.print("FREQ ");
  int32_t freq = SCALE8_16(harmono_graph.get_freq()) + (harmono_graph.cv_freq.value() * 16);
  freq = USAT16(freq);
  graphics.print(freq >> 8);

//  int first_visible_param = HARMONOGRAPH_SETTING_RHO1; 
//
//  UI_START_MENU(kStartX);
//  UI_BEGIN_ITEMS_LOOP(kStartX, first_visible_param, HARMONOGRAPH_SETTING_LAST, harmono_graph_state.selected_param, 0)
//    const settings::value_attr &attr = HarmonoGraph::value_attr(current_item);
//    UI_DRAW_SETTING(attr, harmono_graph.get_value(current_item), kUiWideMenuCol1X);
//  UI_END_ITEMS_LOOP();

  GRAPHICS_END_FRAME();
}


void HARMONOGRAPH_screensaver() {
  GRAPHICS_BEGIN_FRAME(false);

scope_render();

  GRAPHICS_END_FRAME();
}

void HARMONOGRAPH_handleEvent(OC::AppEvent event) {
  switch (event) {
    case OC::APP_EVENT_RESUME:
      encoder[LEFT].setPos(harmono_graph.get_value(HARMONOGRAPH_SETTING_FREQ));
      encoder[RIGHT].setPos(harmono_graph.get_value(harmono_graph_state.selected_param));
      break;
    case OC::APP_EVENT_SUSPEND:
    case OC::APP_EVENT_SCREENSAVER:
      break;
  }
}

void HARMONOGRAPH_topButton() {
//  lorenz_generator.change_value(HARMONOGRAPH_SETTING_FREQ, 32);
//  encoder[LEFT].setPos(harmono_graph.get_value(HARMONOGRAPH_SETTING_FREQ));
  harmono_graph.harmonograph.Init();
}

void HARMONOGRAPH_lowerButton() {
//  lorenz_generator.change_value(HARMONOGRAPH_SETTING_FREQ, -32);
//  encoder[LEFT].setPos(harmono_graph.get_value(HARMONOGRAPH_SETTING_FREQ));
}

void HARMONOGRAPH_rightButton() {
//  ++harmono_graph_state.selected_param;
//  if (harmono_graph_state.selected_param >= HARMONOGRAPH_SETTING_LAST)
//    harmono_graph_state.selected_param = HARMONOGRAPH_SETTING_RHO1;
//  encoder[RIGHT].setPos(harmono_graph.get_value(harmono_graph_state.selected_param));
}

void HARMONOGRAPH_leftButton() {
  encoder[LEFT].setPos(harmono_graph.get_value(HARMONOGRAPH_SETTING_FREQ));
}

void HARMONOGRAPH_leftButtonLong() {
}

bool HARMONOGRAPH_encoders() {
  bool changed = false;
  int value = encoder[LEFT].pos();
  if (value != harmono_graph.get_value(HARMONOGRAPH_SETTING_FREQ)) {
      harmono_graph.apply_value(HARMONOGRAPH_SETTING_FREQ, value);
      encoder[LEFT].setPos(harmono_graph.get_value(HARMONOGRAPH_SETTING_FREQ));
      changed = true;
  }

  value = encoder[RIGHT].pos();
  if (value != harmono_graph.get_value(harmono_graph_state.selected_param)) {
    harmono_graph.apply_value(harmono_graph_state.selected_param, value);
    encoder[RIGHT].setPos(harmono_graph.get_value(harmono_graph_state.selected_param));
    changed = true;
  }

  return changed;
}

void HARMONOGRAPH_debug() {
  graphics.setPrintPos(2, 12);
  graphics.print(harmono_graph.cv_freq.value());
  graphics.print(" ");
  int32_t value = SCALE8_16(harmono_graph.get_freq());
  graphics.print(value);
//  graphics.print(" ");
//  graphics.print(lorenz_generator.cv_shape.value() * 16);
//  value += lorenz_generator.cv_shape.value() * 16;
//  graphics.setPrintPos(2, 22);
//  graphics.print(value); graphics.print(" ");
//  value = USAT16(value);
//  graphics.print(value);
}
