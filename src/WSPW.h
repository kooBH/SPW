#pragma once

#include <QTabWidget>
#include <QWidget>
#include <QApplication>
#include <QHBoxLayout>

#include "WSetting.h"
#include "WSSTT.h"

class WSPW : public QWidget{
  Q_OBJECT

private :
  QHBoxLayout layout_main;
  QTabWidget widget_tab;
  WSSTT widget_sstt;
  WSetting widget_setting;

public :
  WSPW(QWidget *parent = 0);
  ~WSPW();
};