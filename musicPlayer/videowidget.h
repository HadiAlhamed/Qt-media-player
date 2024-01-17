#include <QVideoWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QDebug>
#include<QPainter>
#include<QTimer>
#include <QObject>
#include <QMetaObject>
class VideoWidget : public QVideoWidget
{
    Q_OBJECT
private:
    QString volume_percentage = "0";
    bool display_volume = 0;


public:

    VideoWidget(QWidget *parent = nullptr)
        : QVideoWidget(parent)
    {
        setAcceptDrops(true);

    }

    void set_volume_percentage(QString volume_percentage){
        this->volume_percentage = volume_percentage;
           qDebug() << "Volume percentage set to:" << volume_percentage;
           this->volume_percentage = volume_percentage;
           this->update();

    }

signals:
    void videoDropped(const QString &filePath);
    void VideoClickedon();
    void keyPressed(QKeyEvent *event);
protected:
    void paintEvent(QPaintEvent *event) override{
        // Call the base class paintEvent() method to draw the video
            QVideoWidget::paintEvent(event);
            qDebug() << "paintEvent called!";

            if (display_volume) {
                QPainter painter(this);
                painter.setPen(Qt::white);
                painter.setFont(QFont("Arial", 30));
                painter.drawText(rect(), Qt::AlignCenter, volume_percentage);
            }
            qDebug() << "paintEvent called!";

    }

    void dragEnterEvent(QDragEnterEvent *event) override
    {
        if (event->mimeData()->hasUrls() && event->mimeData()->urls().count() == 1) {
            QUrl url = event->mimeData()->urls().at(0);
            if (url.isLocalFile() && (url.toString().endsWith(".mp4") || (url.toString().endsWith(".mp3")) || (url.toString().endsWith(".mkv")))) {
                event->acceptProposedAction();
            }
        }
    }

    void dropEvent(QDropEvent *event) override
    {
        if (event->mimeData()->hasUrls() && event->mimeData()->urls().count() == 1) {
            QUrl url = event->mimeData()->urls().at(0);
            if (url.isLocalFile() && (url.toString().endsWith(".mp4") || (url.toString().endsWith(".mp3")) || (url.toString().endsWith(".mkv")))) {
                QString filePath = url.toLocalFile();
                qDebug() << "File dropped: " << filePath;
                emit videoDropped(filePath);
            }
        }
    }
    void mouseDoubleClickEvent(QMouseEvent *event) override
        {
            if (event->button() == Qt::LeftButton)
            {
               emit  VideoClickedon();
            }
            event->accept();
         }
    void keyPressEvent(QKeyEvent *event)override {

        emit keyPressed(event);
        QVideoWidget::keyPressEvent(event);
    }
};
