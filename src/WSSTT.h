#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "RtInput.h"
#include "jsonConfig.h"
#include "SSTT.h"

class WSSTT: public QWidget {
  Q_OBJECT

private :
  QVBoxLayout layout_main;
  QHBoxLayout layout_recog;
  QLabel label_head;
  QPushButton btn_run;
  QLabel label_recog;

  int size_request = 32*1024;
  int size_buf = 512;
  int device_1, device_2;
  //int sr = 16000;
  int sr = 48000;

  bool is_running=false;

  RtInput *input_1;
  RtInput *input_2;
  short *buf_1, *buf_2;
  short *buf_req;

  int idx_buf;

  SSTT *sstt;

  void Recording();

public :
  WSSTT(QWidget *parent = 0);
  ~WSSTT();

  void Request();

public slots:
  void Run();
  void UpdateLabel(std::string str);
  void Stop();

signals : 
  void SignalRun();
  void SignalStop();


};