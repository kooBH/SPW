#include "WSSTT.h"

WSSTT::WSSTT(QWidget* parent) : QWidget(parent) {

  layout_recog.addWidget(&label_head);
  layout_recog.addWidget(&btn_run);
  layout_recog.setAlignment(Qt::AlignLeft);

  label_head.setText("Recog");
  btn_run.setText("Run");


  layout_main.addLayout(&layout_recog);
  layout_main.addWidget(&label_recog);
  label_recog.setText("Text here");
  label_recog.setStyleSheet("QLabel { background-color : white;}");

  label_recog.setMinimumWidth(1280);

  QFont font = label_recog.font();
  font.setPointSize(24);
  label_recog.setFont(font);

  setLayout(&layout_main);

  layout_main.setAlignment(Qt::AlignTop);

  QObject::connect(&btn_run, &QPushButton::clicked, [&]() {
    if (!is_running)
      emit(SignalRun());
    else
      emit(SignalStop());
  });

  QObject::connect(this, &WSSTT::SignalRun, this, &WSSTT::Run);
  QObject::connect(this, &WSSTT::SignalStop, this, &WSSTT::Stop);

  buf_1 = new short[size_buf];
  buf_2 = new short[size_buf];
  buf_req = new short[size_request+size_buf];

}

WSSTT::~WSSTT() {
  delete[] buf_1;
  delete[] buf_2;
  delete[] buf_req;

}

void WSSTT::Run() {
  btn_run.setEnabled(false);
  //TODO : thread-safe protections
  is_running = true;


  jsonConfig input(_CONFIG_JSON, "input");
  int device_1 = input["device_1"];
  idx_buf = 0;

  //Run Streaming Speach-to-Text
  sstt = new SSTT("ko-KR", sr, size_request);
  sstt->Run();
  std::thread t1(&SSTT::Read, sstt);

  // Run Mic Thread
  input_1 = new RtInput(device_1, 1, sr, size_buf,size_buf);
  input_1->Start();

  std::thread t2(&WSSTT::Recording, this);
  t1.detach();
  t2.detach();
  btn_run.setEnabled(true);
}

void WSSTT::Recording() {
  while (input_1->IsRunning()) {
    if (input_1->data.stock.load() >= size_buf) {
      input_1->GetBuffer(buf_req+idx_buf);
      idx_buf += size_buf;

      if (idx_buf >= size_request) {
      // Send Request
        sstt->Write(buf_req, size_request);

        // TODO more concurrent routine
        label_recog.setText(QString::fromUtf8(sstt->GetTranscript().c_str()));

        // Relocate request buffer
        idx_buf = 0;
      }
    }
  }
  printf("WSSTT::Recording()::End\n");
}


void WSSTT::UpdateLabel(std::string str) {
  label_recog.setText(str.c_str());
}

void WSSTT::Stop() {
  btn_run.setEnabled(false);
  input_1->Stop();
  input_1->Clear();
  while (input_1->IsRunning());
  sstt->Finish();

  delete input_1;
  delete sstt;

  is_running = false;


  btn_run.setEnabled(true);
}
