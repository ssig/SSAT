#include "workerthread.h"

WorkerThread::WorkerThread()
{
    this->playing = false;
}

WorkerThread::~WorkerThread()
{
    this->clearVideo();
}

Mat WorkerThread::getFrame()
{
    Mat frame;
    this->video >> frame;
    return frame;
}

Mat WorkerThread::getFrame(double _frameId)
{
    Mat frame;
    this->video.set(CV_CAP_PROP_POS_FRAMES, _frameId);
    this->video >> frame;
    return frame;
}

double WorkerThread::getFrameId()
{
    return this->video.get(CV_CAP_PROP_POS_FRAMES);
}

double WorkerThread::getTotalFrames()
{
    return this->video.get(CV_CAP_PROP_FRAME_COUNT);
}

double WorkerThread::getVideoFPS()
{
    return this->video.get(CV_CAP_PROP_FPS);
}

bool WorkerThread::isPlaying()
{
    return this->playing;
}

void WorkerThread::isPlaying(bool _condition)
{
    this->playing = _condition;
}

void WorkerThread::clearVideo()
{
    this->video.release();
}

void WorkerThread::loadVideo(QString _path)
{
    this->video.open(_path.toStdString());
}

void WorkerThread::exportJSON(const Core &_core)
{
    //Breno e Danilo: adicioanar implementação aqui
}

void WorkerThread::importJSON(Core &_core, const QString &_path)
{
    //Breno e Danilo: adicioanar implementação aqui
}

QImage WorkerThread::matToQimage(const Mat &_frameId)
{
    if (_frameId.type() == CV_8UC3)
    {
        const uchar *qImageBuffer = const_cast<uchar*>(_frameId.data);
        QImage img(qImageBuffer, _frameId.cols, _frameId.rows, static_cast<int>(_frameId.step), QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        qErrnoWarning("ERROR: Mat could not be converted to QImage.");
        return QImage();
    }
}

void WorkerThread::allotFrameBasedSegment(Core &_singleton, const FrameBasedData &_data)
{
    for(int frameId = _data.getInitialFrameId(); frameId <= _data.getFinalFrameId(); frameId++)
    {
        unsigned long longFrameId = static_cast<unsigned long>(frameId);
        _singleton.frames[longFrameId].setName(_data.getName());
        _singleton.frames[longFrameId].addAttributes("_data.getCategory()", "_data.getLabel()");
//        _singleton.frames[longFrameId].addAttributes(_data.getCategory(), _data.getLabel());
    }
}
