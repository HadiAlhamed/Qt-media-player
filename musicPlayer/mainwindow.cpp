#include "mainwindow.h"
#include "ui_mainwindow.h"
/////////---------------library
///

/////////////-------library
///
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QIcon icon(":/ic/icons8-media-player-64.ico");
    setWindowIcon(icon);
    player = new MediaPlayer(this);

    player->setVideoOutput(ui->video_widget); // render the video to the video widget
    // ---------------library


//    // Create a new model object
    model = new QStandardItemModel(this);
    ui->playList->setModel(model);
    //-------------------
    slider = new QSlider(this);
    slider->setStyleSheet("QSlider::groove:horizontal { border: 1px solid #999999; height: 8px; background: #FCE7CC; } QSlider::handle:horizontal { background-color: #000000; width: 16px; margin: -5px 0; border-radius: 4px; }");
    slider ->setOrientation(Qt :: Horizontal);
    slider->setRange(0, 100);

    ui->toolBar_3->addWidget(slider);
    connect(ui->video_widget, &VideoWidget::videoDropped, this, [this](const QString &filePath) {
        QFileInfo fileInfo(filePath);
        if (fileInfo.exists() && fileInfo.isFile() && (fileInfo.suffix().toLower() == "mkv" || fileInfo.suffix().toLower() == "mp4" ||  fileInfo.suffix().toLower() == "mp3")){
            qDebug() << "the dropped file path : " << filePath << "\n";
            QStandardItem* item = new QStandardItem(fileInfo.fileName());
            item->setData(QVariant(filePath), Qt::UserRole);
            item->setEditable(0);
            QPainter painter(this);
            painter.setPen(Qt::NoPen);
            model->appendRow(item);
            int rowCount = model->rowCount();
            int colCount = model->columnCount();
            QModelIndex last_index = model->index(rowCount - 1 ,colCount - 1);
            qDebug() << "row count =  " << rowCount  << "\n";
            ui->playList->setCurrentIndex(last_index);
            on_playList_doubleClicked(last_index);
        }
    });
    connect(player,&MediaPlayer ::durationChanged,slider,&QSlider :: setMaximum);
    connect(player,&MediaPlayer ::positionChanged,slider,&QSlider :: setValue);
    connect(slider,&QSlider ::sliderMoved,player,&MediaPlayer :: setPosition);
    connect(slider, &QSlider::valueChanged, this, &MainWindow::onValueChanged);
    connect(ui->video_widget , &VideoWidget :: VideoClickedon , this , [this](){
                   on_actionfullScreen_triggered();
            });
    connect(ui->video_widget , &VideoWidget::keyPressed , this , [this](QKeyEvent *event)
    {
        this->handle_key_pressed(event);
    });

    //show duration of the song/video
    duration_label = new QLabel(ui->toolBar_3);
    ui->toolBar_3->addWidget(duration_label);
    connect(player, &MediaPlayer::positionChanged, [=](qint64 position) {
        // Get the total duration of the song in milliseconds
        qint64 totalDuration = player->duration();

        // Convert the position and duration to hours, minutes, and seconds
        int hoursPassed = (position / (1000 * 60 * 60)) % 24;
        int minutesPassed = (position / (1000 * 60)) % 60;
        int secondsPassed = (position / 1000) % 60;
        int hoursTotal = (totalDuration / (1000 * 60 * 60)) % 24;
        int minutesTotal = (totalDuration / (1000 * 60)) % 60;
        int secondsTotal = (totalDuration / 1000) % 60;

        // Format the current position and total duration as strings in the format hh:mm:ss
        QString positionStr = QTime(hoursPassed, minutesPassed, secondsPassed).toString("hh:mm:ss");
        QString totalDurationStr = QTime(hoursTotal, minutesTotal, secondsTotal).toString("hh:mm:ss");

        // Update the label text to show the current position and total duration
        duration_label->setText(positionStr + " / " + totalDurationStr);
    });
    QLabel* label = new QLabel("Hello World", ui->video_widget);
    label->setStyleSheet("background-color: transparent; color: white; font-size: 30pt;");

    // Position the label on top of the video widget
    label->setGeometry(10, 10, 200, 50); // Set the position and size of the label

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionstop_triggered()
{
    player->stop();
    is_on = 0;
    ui->actionplay->setChecked(0);
    ui->actionpause->setChecked(0);
    ui->actionstop->setChecked(1);
    ui->statusbar->showMessage("Stopped...");
    slider->setFocus();

}

void MainWindow::on_actionplay_triggered()
{
    player->play();
    is_on = 1;
    ui->actionplay->setChecked(1);
    ui->actionpause->setChecked(0);
    ui->actionstop->setChecked(0);
    ui->statusbar->showMessage("Playing...");
    slider->setFocus();

}


void MainWindow::on_actionpause_triggered()
{
    player->pause();
    is_on = 0;
    ui->actionplay->setChecked(0);
    ui->actionpause->setChecked(1);
    ui->statusbar->showMessage("Paused...");
    slider->setFocus();

}



void MainWindow::on_actionopenfile_triggered()
{
    filename = new QString(QFileDialog::getOpenFileName(this , "open a file" , "" , "All Files (*.*)" ));
    if(filename->isNull())return;
    QFileInfo fileInfo(*filename);

    if (!fileInfo.exists() || !fileInfo.isFile() || !(fileInfo.suffix().toLower() == "mp4" ||  fileInfo.suffix().toLower() == "mp3" || fileInfo.suffix().toLower() == "mkv"))return;
    QStandardItem* item = new QStandardItem(fileInfo.fileName());
    item->setData(QVariant(*filename), Qt::UserRole);
    item->setEditable(0);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    model->appendRow(item);
    int rowCount = model->rowCount();
    int colCount = model->columnCount();
    QModelIndex last_index = model->index(rowCount - 1 ,colCount - 1);
    qDebug() << "row count =  " << rowCount  << "\n";
    ui->playList->setCurrentIndex(last_index);
    on_playList_doubleClicked(last_index);
    //we have to check whether it's valid , playable , etc...

}


void MainWindow::on_actionvolume_up_triggered()
{
    if(is_muted)
    {
        is_muted = 0;
        current_volume = before_mute;
    }
    current_volume = qMin(current_volume + 5 , 100);
    ui->actionmute->setChecked(0);
    player->setVolume(current_volume);
    QString vp = QString::number(current_volume);
    vp += " %";
    ui->video_widget->set_volume_percentage(vp);
    ui->statusbar->showMessage(vp);
    slider->setFocus();

}



void MainWindow::on_actionvolume_down_triggered()
{
    ui->toolBar_3->setFocus();
    if(is_muted)
    {
        is_muted = 0;
        current_volume = before_mute;
    }
    current_volume = qMax(current_volume - 5 , 0);
    player->setVolume(current_volume);
    QString vp = QString::number(current_volume);
    vp += " %";
    ui->video_widget->set_volume_percentage(vp);
    ui->statusbar->showMessage(vp);
    ui->actionmute->setChecked(0);
    slider->setFocus();


}



void MainWindow::on_actionmute_triggered()
{
    if(is_muted){

        player->setVolume(before_mute);
        current_volume = before_mute;
        is_muted = 0;
    }else{

        before_mute = current_volume;
        player->setVolume(0);
        is_muted = 1;
    }
    slider->setFocus();

}

void MainWindow::on_playList_doubleClicked(const QModelIndex &index)
{
    filename = new QString(index.data(Qt::UserRole).toString());
    QFileInfo fileInfo(*filename);
    on_actionstop_triggered();
    player->setMedia(QUrl::fromLocalFile(*filename));//media to play;
    on_actionplay_triggered();
}



//toolbar appears when on FullScreen mode:

void MainWindow::on_actionfullScreen_triggered()
{

    is_fullscreen = 1 - is_fullscreen;
    if (is_fullscreen) {
          // Set the status bar to be a floating widget
          ui->video_widget->setFullScreen(true);
          ui->actionfullScreen->setChecked(1);
          ui->toolBar_3->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
          QDesktopWidget *desktop = QApplication::desktop();
         int screenWidth = desktop->width();
         int screenHeight = desktop->height();
         // Set the position of the status bar
         int toolBarWidth = ui->toolBar_3->width();
         int toolBarHeight = ui->toolBar_3->height();
         int x = (screenWidth - toolBarWidth) / 2; // Center horizontally
         int y = screenHeight - toolBarHeight; // Bottom of the screen
         ui->toolBar_3->move(x, y);
         ui->toolBar_3->show();
      }else{
          // Restore the status bar to its normal position
          ui->video_widget->setFullScreen(false);
          ui->actionfullScreen->setChecked(0);
          ui->toolBar_3->setWindowFlags(Qt::Widget);
          ui->toolBar_3->show();
      }
    slider->setFocus();

}

void MainWindow::on_actionprevious_triggered()
{
    if(model->rowCount() == 0){
        if(filename->isNull())
        {
            QMessageBox::information(this, "message" , "there is no other media files");

        }
        return;
    }
    QModelIndex current_index = ui->playList->currentIndex();
    QStandardItem *current_item = model->itemFromIndex(current_index);
    int rowCount = model->rowCount();
    int colCount = model->columnCount();
    QModelIndex last_index = model->index(rowCount - 1 ,colCount - 1);
    QModelIndex first_index = model->index(0,0);
    QModelIndex prev_index = current_item->index().sibling(current_index.row() - 1, current_index.column());
    if(shuffle){
        int minVal = 0;
        int maxVal = rowCount - 1;
        int randomNum = qrand() % ((maxVal + 1) - minVal) + minVal;
        prev_index = current_item->index().sibling(randomNum,0);
    }
    else if (current_index == first_index) {
        if(loop_over_all || loop_over_song){
            prev_index = last_index;
        }else{
            QMessageBox::information(this, "message" , "this is the first media file");
            return;
        }
    }
    ui->playList->setCurrentIndex(prev_index);
    on_playList_doubleClicked(prev_index);
    slider->setFocus();

}

void MainWindow::on_actionnext_triggered()
{
    if(model->rowCount() == 0){
        if(filename->isNull())
        {
            QMessageBox::information(this, "message" , "there is no other media files");

        }
        return;
    }
    qDebug() << "model rows = " << model->rowCount() << "\n";

    QModelIndex current_index = ui->playList->currentIndex();
    QStandardItem *current_item = model->itemFromIndex(current_index);
    qDebug() <<" current index = " << current_index.row() << "\n";
    int rowCount = model->rowCount();
    int colCount = model->columnCount();
    QModelIndex last_index = model->index(rowCount - 1 ,colCount - 1);
    QModelIndex first_index = model->index(0,0);
    QModelIndex next_index = current_item->index().sibling(current_index.row() + 1, current_index.column());
    if(isChanged && loop_over_song){
        next_index = current_index;
    }
    else if(shuffle){
        int minVal = 0;
        int maxVal = rowCount - 1;
        int randomNum = qrand() % ((maxVal + 1) - minVal) + minVal;
        next_index = current_item->index().sibling(randomNum,0);
    }
    else if (current_index == last_index) {
        if(loop_over_all || loop_over_song){
            next_index = first_index;
        }else{
            QMessageBox::information(this, "message" , "this is the last media file");
            return;
        }
    }

    ui->playList->setCurrentIndex(next_index);
    on_playList_doubleClicked(next_index);
    slider->setFocus();

}

void MainWindow::on_actionLoopOverSong_triggered()
{
    if(loop_over_song){
        loop_over_song = 0;
    }else{
        loop_over_song = 1;
        loop_over_all = 0;
        ui->actionLoopOverAll->setChecked(0);
    }
    slider->setFocus();

}

void MainWindow::on_actionLoopOverAll_triggered()
{
    if(loop_over_all){
        loop_over_all = 0;
    }else{
        loop_over_all = 1;
        loop_over_song = 0;
        ui->actionLoopOverSong->setChecked(0);
    }
    slider->setFocus();

}

void MainWindow::on_actionshuffle_triggered()
{
    if(shuffle){
        shuffle = 0;
    }else{
        shuffle = 1;
    }
    slider->setFocus();

}


//when slider reaches 100% handle next song
void MainWindow::onValueChanged(int value){
    isChanged = 1;
    if(slider && value == slider->maximum()) {

        on_actionnext_triggered();
        isChanged = 0;
    }

}

void MainWindow::on_actionopen_folder_triggered()
{

        folder_path = QFileDialog::getExistingDirectory(nullptr, "Open Folder", QDir::homePath());
        songPaths.clear();
//        model->clear();
        if (!folder_path.isEmpty()) {
            // Do something with the selected folder path
            songPaths = getSongPaths();
            foreach(QString songPath, songPaths) {
                QFileInfo fileInfo(songPath);
                QString extension = fileInfo.suffix().toLower();
                if (extension == "mp3" || extension == "mp4" || extension == "mkv") {
                    QStandardItem* item = new QStandardItem(fileInfo.fileName());
                    item->setData(QVariant(songPath), Qt::UserRole);
                    item->setEditable(0);
                    QPainter painter(this);
                    painter.setPen(Qt::NoPen);
                    model->appendRow(item);
                }
            }
        }

}


void MainWindow::on_actionskip_forward_triggered()
{
    qint64 cur_pos = player->position();
    cur_pos = qMin(cur_pos + 5000 , player->duration() );
    player->setPosition(cur_pos);
    slider->setFocus();

}

void MainWindow::on_actionskip_backward_triggered()
{
    qint64 cur_pos = player->position();
    cur_pos = qMax(cur_pos - 5000 , (qint64)0);
    player->setPosition(cur_pos);
    slider->setFocus();

}

void MainWindow::on_actionclear_playlist_triggered()
{
    on_actionstop_triggered();
    model->clear();

    // Format the current position and total duration as strings in the format hh:mm:ss
    QString positionStr = QTime(0, 0, 0).toString("hh:mm:ss");
    QString totalDurationStr = QTime(0, 0, 0).toString("hh:mm:ss");
    qDebug() << "position str = " << positionStr <<"\n";
    qDebug() << "total duration str = " << totalDurationStr <<"\n";
    // Update the label text to show the current position and total duration
    duration_label->setText(positionStr + " / " + totalDurationStr);

}
