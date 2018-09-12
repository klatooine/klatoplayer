#ifndef KLATOPLAYER_H
#define KLATOPLAYER_H

#include <QMainWindow>
#include <QFileDialog>
#include <player.h>
#include <QStringList>
#include <QString>
#include <QListWidget>
#include <QListWidgetItem>

namespace Ui {
class klatoplayer;
}

class klatoplayer : public QMainWindow {
  Q_OBJECT

public:
  explicit klatoplayer(QWidget *parent = nullptr);
  ~klatoplayer();

private slots:
  void on_addMediaButton_clicked();
  void on_playButton_clicked();
  void on_stopButton_clicked();
  void on_pauseButton_clicked();
  void on_nextButton_clicked();
  void on_previousButton_clicked();
  void PositionChangedByPlayer(qint64 position);
  void PositionChangedByUser(int position);
  void CurrentSongChangedByPlayer(int playlistIndex);
  void CurrentSongChangedByUser();
  void on_savePlaylistButton_clicked();
  void on_loadPlaylistButton_clicked();
  void UpdateSongList();

  void on_removeMediaButton_clicked();

  void on_volumeSlider_valueChanged(int value);

private:
  Ui::klatoplayer *ui;
  Player * player_ = Player::GetInstance();
  Playlist * playlist_ = new Playlist;

  void SetPlayerPosition(qint64 position);

  QString MillisecondsTo24HString(qint64 milliseconds);
};

#endif // KLATOPLAYER_H
