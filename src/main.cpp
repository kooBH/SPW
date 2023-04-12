#include <QApplication>

#include "RtInput.h"
#include "WAV.h"
#include "WSPW.h"


/*
Test 
*/
int main(int argc, char* argv[]) {
  //path to dll 
  QCoreApplication::addLibraryPath(".");

  //path to so
  QCoreApplication::addLibraryPath("../lib");
  //QCoreApplication::addLibraryPath("../lib/platfrominputcontexts");

  QApplication app(argc, argv);

  printf("Loading....\n");
  WSPW spw;

  app.setWindowIcon(QIcon("../resource/record.ico"));

  spw.show();

  return app.exec();

  /*
  int device_1 = 8;
  int device_2 = 9;

  int sr_in = 16000;
  int sr_out = 16000;

  int n_hop = 512;

  short* buf_1 = new short[n_hop];
  short* buf_2 = new short[n_hop];
  short* buf = new short[2*n_hop];

  AudioProbe();

  RtInput input_1(device_1, 1, sr_in,512,n_hop);
  RtInput input_2(device_2, 1, sr_in,512,n_hop,n_hop);

  input_1.Start();
  input_2.Start();


  WAV out(2,sr_out);
  out.NewFile("out.wav");

  int cnt = 0;
  int read;

  while (input_1.IsRunning()) {
    if (input_1.data.stock.load() >= n_hop && input_2.data.stock.load() >= n_hop) {
       input_1.GetBuffer(buf_1);
       input_2.GetBuffer(buf_2);

       for (int i = 0; i < n_hop; i++) {
          buf[2 * i] = buf_1[i];
          buf[2 * i+1] = buf_2[i];
       }

        out.Append(buf,n_hop*2);
        cnt++;
    }
    else {
      SLEEP(10);
    }

    if (cnt > 300)
      break;
  }

  out.Finish();
  */

  return 0;
}