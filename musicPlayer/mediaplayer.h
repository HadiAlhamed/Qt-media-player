#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H
#include<QMediaPlayer>
#include<QKeyEvent>
#include<QObject>
class MediaPlayer : public QMediaPlayer
{
    Q_OBJECT

public:
    explicit MediaPlayer(QObject *parent = nullptr) : QMediaPlayer(parent){}

};
#endif // MEDIAPLAYER_H
