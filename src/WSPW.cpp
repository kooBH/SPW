#include "WSPW.h"

WSPW::WSPW(QWidget* parent) {
  widget_tab.addTab(&widget_sstt, "SSTT");
  widget_tab.addTab(&widget_setting, "Setting");
  layout_main.addWidget(&widget_tab);
  setLayout(&layout_main);

  layout_main.setAlignment(Qt::AlignTop);

}

WSPW::~WSPW() {

}

