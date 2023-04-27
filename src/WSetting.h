#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTextBrowser>
#include <QMessageBox>

#include "RtBase.h"
#include "jsonConfig.h"

using std::string;
using std::to_string;
using std::map;
using std::make_pair;

class WSetting : public QWidget {
  Q_OBJECT
private : 
  QVBoxLayout base_layout;
  QTextBrowser TB_device;
  QPushButton btn_reprobe;

  QHBoxLayout layout_1;
  QHBoxLayout layout_2;

  QLabel label_mic_1;
  QLabel label_sr_1;
  QLabel label_mic_2;
  QLabel label_sr_2;

  QComboBox combo_mic_1;
  QComboBox combo_sr_1;
  QComboBox combo_mic_2;
  QComboBox combo_sr_2;
  map<string, unsigned int> map_device;
  QString text_device;

  inline std::string mid_num_str(const std::string& s) {

    if (!s.compare(""))
      printf("WSetting::mid_num_str()::Item not selected. Using default index 0\n");
      return "0";

    std::string::size_type p = s.find('[');
    std::string::size_type pp = s.find(']', p + 2);
    return s.substr(p + 1, pp - p - 1);
  }

public :
  WSetting(QWidget *parent = 0);
  ~WSetting();

  void AudioProbe();

};