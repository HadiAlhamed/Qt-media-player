#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include<QString>
#include <QSplitter>
#include <videowidget.h>
#include<QMediaPlayer>
#include<QTime>
#include<QFileDialog>
#include<QProgressBar>
#include<QSlider>
#include<QStringList>
#include<QFileInfoList>
#include<QDir>
#include<QMessageBox>
#include<QObject>
#include<QDirIterator>
#include<QStandardItemModel>
#include<QStandardItem>
#include<QModelIndex>
#include<QStyleHints>
#include<QMediaService>
#include<QDesktopWidget>
#include<QModelIndex>
#include<QtGlobal>
#include<QPalette>
#include<QPixmap>
#include<QImage>
#include<QIcon>
#include<QWidget>
#include<QMouseEvent>
#include<QKeyEvent>
#include<QPoint>
#include<QLabel>
#include<QTimer>
#include<QFont>
#include<QPainter>
#include<QMouseEvent>
#include<QWidget>
#include<mediaplayer.h>
#include<QGraphicsView>
#include<QGraphicsScene>
#include<QGraphicsTextItem>
#include<QGraphicsPixmapItem>
#include<graphicsview.h>
#include<QGraphicsProxyWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QStringList getSongPaths()
    {
        QStringList songPaths;
        QDirIterator it(folder_path, QStringList() << "*.mp3" << "*.mp4" << "*.mkv", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            it.next();
            songPaths.append(it.filePath());
        }

        return songPaths;

    }

    void handle_open_with(QString fileName){
        filename = new QString(fileName);
        on_actionstop_triggered();
        player->setMedia(QUrl::fromLocalFile(*filename));//media to play;
        //we have to check whether it's valid , playable , etc...
        on_actionplay_triggered();
    }
    void handle_key_pressed(QKeyEvent *event)
    {
        keyPressEvent(event);
    }
protected:
    void keyPressEvent(QKeyEvent *event)override {


        if (event->type() == QEvent::KeyPress) {
                    // Call your function here

        qDebug() << "key is pressed " << event->key() << "\n";
        switch (event->key()) {

                    case Qt::Key_2:
                    {

                        emit on_actionvolume_up_triggered();
                        event->accept();
                        break;
                    }
                    case Qt::Key_1:
                    {

                         emit on_actionvolume_down_triggered();
                            event->accept();
                         break;
                    }
                    case Qt::Key_Space:
                    {

                        if(is_on){
                            on_actionpause_triggered();
                        }else{
                            on_actionplay_triggered();
                        }
                        event->accept();
                        break;
                     }
                    case Qt::Key_4:
                    {

                        on_actionskip_forward_triggered();
                        qDebug() << "New position:" << player->position() << "\n";

                    }
                    case Qt::Key_3:
                    {

                        on_actionskip_backward_triggered();
                         qDebug() << "New position:" <<  player->position() << "\n";

                    }

                    default:
                    {
                        QMainWindow::keyPressEvent(event);
                        break;
                    }
                }
            }
        }






private slots:
    void on_actionplay_triggered();

    void on_actionpause_triggered();

    void on_actionopenfile_triggered();

    void on_actionstop_triggered();

    void on_actionvolume_up_triggered();

    void on_actionvolume_down_triggered();

    void on_actionmute_triggered();

    void on_playList_doubleClicked(const QModelIndex &index);

    void on_actionnext_triggered();

    void on_actionprevious_triggered();

    void on_actionLoopOverSong_triggered();

    void on_actionLoopOverAll_triggered();

    void on_actionshuffle_triggered();

    void onValueChanged(int value);

    void on_actionopen_folder_triggered();

    void on_actionfullScreen_triggered();

    void on_actionskip_forward_triggered();

    void on_actionskip_backward_triggered();

    void on_actionclear_playlist_triggered();

private:
    Ui::MainWindow *ui;
    MediaPlayer* player;
    QString* filename;
    QProgressBar* bar;
    QSlider* slider;
    QStandardItemModel* model;
    QString folder_path;
    QStringList songPaths;
    QLabel* duration_label;
    int current_volume = 0;
    int before_mute = 0;
    bool is_muted = 0;
    bool is_fullscreen = 0;
    bool loop_over_all = 0 , loop_over_song = 0 , shuffle = 0;
    bool isChanged = 0;
    bool is_on = 0;

};
#endif // MAINWINDOW_H
