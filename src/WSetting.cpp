#include "WSetting.h"


WSetting::WSetting(QWidget* parent)
  : QWidget(parent), btn_reprobe("Audio Probe") {

  AudioProbe();

  TB_device.setText(text_device);
  base_layout.addWidget(&btn_reprobe);

  layout_1.addWidget(&label_1);
  layout_1.addWidget(&combo_1);
  layout_1.setAlignment(Qt::AlignLeft);
  base_layout.addLayout(&layout_1);

  layout_2.addWidget(&label_2);
  layout_2.addWidget(&combo_2);
  layout_2.setAlignment(Qt::AlignLeft);
  base_layout.addLayout(&layout_2);

  base_layout.addWidget(&TB_device);

  base_layout.setAlignment(Qt::AlignTop);

  base_layout.addWidget(&TB_device);

  setLayout(&base_layout);

  label_1.setText("MIC 1");
  label_2.setText("MIC 2");

  QObject::connect(&btn_reprobe, &QPushButton::clicked,
    [&]() {
      AudioProbe();
      TB_device.setText(text_device);

    }
  );

  QObject::connect(&combo_1, &QComboBox::currentTextChanged,
    [&](const QString& item) {
      std::ifstream ifs(_CONFIG_JSON);
      json jj = json::parse(ifs);
      ifs.close();

      jj["input"]["device_1"] = std::stoi(mid_num_str(item.toStdString()));
      std::ofstream ofs(_CONFIG_JSON);
      ofs << jj.dump(4);
      ofs.close();
    }
  );

  QObject::connect(&combo_2, &QComboBox::currentTextChanged,
    [&](const QString& item) {
      std::ifstream ifs(_CONFIG_JSON);
      json jj = json::parse(ifs);
      ifs.close();

      jj["input"]["device_2"] = std::stoi(mid_num_str(item.toStdString()));
      std::ofstream ofs(_CONFIG_JSON);
      ofs << jj.dump(4);
      ofs.close();
    }
  );

  std::ifstream ifs(_CONFIG_JSON);
  if (!ifs.is_open()) {
    QMessageBox::critical(
      this,
      tr("KInput"),
      tr("Can't Open Config.json File!"));
    exit(-1);
  }

  json j = json::parse(ifs);
  ifs.close();

  combo_1.setCurrentIndex(j["input"]["device_1"].get<int>());
  combo_2.setCurrentIndex(j["input"]["device_2"].get<int>());
}

WSetting::~WSetting(){

}

void WSetting::AudioProbe() {

  map_device.clear();

  text_device = " *** Device List *** \n\n";
  // Create an api map.
  std::map<int, std::string> apiMap;
  apiMap[RtAudio::MACOSX_CORE] = "OS-X Core Audio";
  apiMap[RtAudio::WINDOWS_ASIO] = "Windows ASIO";
  apiMap[RtAudio::WINDOWS_DS] = "Windows Direct Sound";
  apiMap[RtAudio::WINDOWS_WASAPI] = "Windows WASAPI";
  apiMap[RtAudio::UNIX_JACK] = "Jack Client";
  apiMap[RtAudio::LINUX_ALSA] = "Linux ALSA";
  apiMap[RtAudio::LINUX_PULSE] = "Linux PulseAudio";
  apiMap[RtAudio::LINUX_OSS] = "Linux OSS";
  apiMap[RtAudio::RTAUDIO_DUMMY] = "RtAudio Dummy";

  RtAudio audio;
  RtAudio::DeviceInfo info;
  text_device.append("Current API : ");
  text_device.append(
    QString::fromStdString(apiMap[audio.getCurrentApi()]));
  text_device.append("\n\n");

  unsigned int devices = audio.getDeviceCount();

  //text_device = "";

  /* Create Widgets */
  for (unsigned int i = 0; i < devices; i++) {
    info = audio.getDeviceInfo(i);
    QString temp_device = "[";
    temp_device.append(QString::fromStdString(to_string(i)));
    temp_device.append("]");
    temp_device.append(QString::fromStdString(info.name));
    map_device.insert(make_pair(temp_device.toStdString(), i));

    text_device.append(temp_device);

    text_device.append("\n");
    if (info.probed == false) {
      text_device.append("Probe Status = Unsuccessful");
      text_device.append("\n");
    }
    else {
      //  std::cout << "Probe Status = Successful\n";
      text_device.append("Output Channels = ");
      text_device.append(
        QString::fromStdString(to_string(info.outputChannels)));
      text_device.append("\nInput Channels = ");
      text_device.append(
        QString::fromStdString(to_string(info.inputChannels)));
      text_device.append("\n");
      if (info.sampleRates.size() < 1) {
        text_device.append("No supported sample rates found!");
      }
      else {
        text_device.append("Supported sample rates = ");
        for (unsigned int j = 0; j < info.sampleRates.size(); j++) {
          text_device.append(QString::fromStdString(
            to_string(info.sampleRates[j])
          ));
          text_device.append(" ");
        }
      }
      text_device.append("\n");
    }
    text_device.append("\n");
  }
  text_device.append("\n");

  /*** ReCreate Combobox for input device ***/
  int cnt_1 = combo_1.count();
  int cnt_2 = combo_2.count();
  int idx_1 = 0;
  int idx_2 = 0;
  //combo.clear();
  for (auto it = map_device.begin(); it != map_device.end(); it++) {
    if (idx_1 < cnt_1)
      combo_1.setItemText(idx_1, QString::fromStdString(it->first));
    else
      combo_1.addItem(QString::fromStdString(it->first));

    if (idx_2 < cnt_2)
      combo_2.setItemText(idx_2, QString::fromStdString(it->first));
    else
      combo_2.addItem(QString::fromStdString(it->first));

    idx_1++;
    idx_2++;
  }

  for (int i = 0; i < cnt_1 - idx_1; i++)
    combo_1.removeItem(idx_1);
  for (int i = 0; i < cnt_2 - idx_2; i++)
    combo_2.removeItem(idx_2);
}