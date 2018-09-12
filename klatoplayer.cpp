#include "klatoplayer.h"
#include "ui_klatoplayer.h"

klatoplayer::klatoplayer(QWidget *parent):QMainWindow(parent), ui(new Ui::klatoplayer) {
  player_->SetPlaylist(playlist_);

  ui->setupUi(this);

  player_->SetVolume(ui->volumeSlider->value());

  connect(player_, SIGNAL(PositionChanged(qint64)), this, SLOT(PositionChangedByPlayer(qint64)));
  connect(player_, SIGNAL(CurrentSongChanged(int)), this, SLOT(CurrentSongChangedByPlayer(int)));
  connect(ui->timeSlider, SIGNAL(sliderMoved(int)), this, SLOT(PositionChangedByUser(int)));
  connect(ui->songList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(CurrentSongChangedByUser()));
  connect(playlist_, SIGNAL(MediaChanged()), this, SLOT(UpdateSongList()));
}

klatoplayer::~klatoplayer() {
  delete ui;
}

void klatoplayer::on_addMediaButton_clicked() {
  QStringList files = QFileDialog::getOpenFileNames(this, tr("mp3 files"), QDir::currentPath(), tr("mp3 files (*.mp3)"));

  if(!files.isEmpty() && playlist_) {
    for (QString song : files) {
      playlist_->AddSong(song);
    }
  }
}

void klatoplayer::on_playButton_clicked() {
  player_->Play();
}

void klatoplayer::on_stopButton_clicked() {
  player_->Stop();
}

void klatoplayer::on_nextButton_clicked() {
  player_->NextSong();
}

void klatoplayer::on_pauseButton_clicked() {
  player_->Pause();
}

void klatoplayer::on_previousButton_clicked() {
  player_->PreviousSong();
}

QString klatoplayer::MillisecondsTo24HString(qint64 milliseconds) {
  int hours = static_cast<int>(milliseconds / 3600000);
  int minutes = static_cast<int>(milliseconds / 60000) - hours * 60;
  int seconds = static_cast<int>(milliseconds / 1000) - minutes * 60;
  QString result = QString().sprintf("%02d:%02d:%02d", hours, minutes, seconds);

  return result;
}

void klatoplayer::SetPlayerPosition(qint64 position) {
  player_->SetPosition(position);
}

void klatoplayer::PositionChangedByPlayer(qint64 position) {
  QString elapsedTime = "00:00:00";
  double duration = player_->GetCurrentSongDuration();
  int currentPosition = 0;

  if (duration > 0) {
    currentPosition = static_cast<int>((position / duration) * 1000);

    if (currentPosition >= 0 && currentPosition <= 1000) {
      ui->timeSlider->setValue(currentPosition);
    }

    elapsedTime = MillisecondsTo24HString(position);
  }

  ui->label->setText(elapsedTime);
}

void klatoplayer::PositionChangedByUser(int position) {
  qint64 duration = player_->GetCurrentSongDuration();
  int changedPosition = static_cast<int>((position / 1000.0) * duration);

  if (changedPosition > 0 && changedPosition <= duration) {
    SetPlayerPosition(changedPosition);
  }
}

void klatoplayer::CurrentSongChangedByPlayer(int playListIndex) {
  for (int i = 0; i < ui->songList->count(); i++) {
    ui->songList->item(i)->setBackgroundColor(Qt::white);
  }

  ui->songList->item(playListIndex)->setBackgroundColor(Qt::yellow);
}

void klatoplayer::CurrentSongChangedByUser() {
  int currentIndex = ui->songList->currentRow();

  player_->SetSong(currentIndex);
  player_->Play();
}

void klatoplayer::on_savePlaylistButton_clicked()
{
  QString playlist = QFileDialog::getSaveFileName(this, tr("m3u files"), QDir::currentPath(), tr("m3u files (*.m3u)"));

  playlist_->Save(QUrl::fromLocalFile(playlist));
}

void klatoplayer::on_loadPlaylistButton_clicked()
{
  QString playlist = QFileDialog::getOpenFileName(this, tr("m3u files"), QDir::currentPath(), tr("m3u files (*.m3u)"));

  playlist_->Load(QUrl::fromLocalFile(playlist));
}

void klatoplayer::UpdateSongList() {
  ui->songList->clear();

  if (playlist_) {
    if(playlist_->MediaCount() > 0) {
      for (int i = 0; i < playlist_->MediaCount(); i++) {
        ui->songList->addItem(playlist_->GetPlaylist()->media(i).canonicalUrl().fileName());
      }
    }
  }
}

void klatoplayer::on_removeMediaButton_clicked()
{
  int currentIndex = ui->songList->currentRow();

  playlist_->RemoveSong(currentIndex);
}

void klatoplayer::on_volumeSlider_valueChanged(int value)
{
  player_->SetVolume(value);
}
