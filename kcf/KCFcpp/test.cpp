#include <stdlib.h>

#include <memory>
#include<iostream>
//#include<opencv2/core.hpp>
//#include <opencv2/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<list>
#include<ctime>
#include "kcftracker.hpp"
#define True 1
#define False 0

class Track_Info{
public:
    cv::Mat image;
    std::list<cv::Rect> regions;
    Track_Info(cv::Mat & img){
        image = img.clone();
    }

    ~Track_Info(){}
};

void callback_fun(int event, int x, int y, int flags, void* info){
    static bool draw = False;
    static int ix,iy;
    static cv::Rect temp_region;
    cv::Mat temp_img;
    auto temp_info = (Track_Info*)info;
    temp_img = temp_info->image.clone();
    if(event == cv::EVENT_LBUTTONDOWN){
        draw = True;
        ix = x;
        iy = y;
    }
    else if(event == cv::EVENT_MOUSEMOVE && draw){
        temp_region.x = std::min(ix,x);
        temp_region.y = std::min(iy,y);
        temp_region.height = std::abs(iy-y);
        temp_region.width = std::abs(ix - x);
        cv::rectangle(temp_img,temp_region,cv::Scalar(0,0,255),2);
        for (auto iter = temp_info->regions.begin(); iter != temp_info->regions.end(); iter++){
            cv::rectangle(temp_img,*iter,cv::Scalar(0,0,255),2);
        }
    }
    else if(event == cv::EVENT_LBUTTONUP){
        draw = False;
        temp_info->regions.push_back(temp_region);
    }
    if(draw){
        cv::imshow("frame",temp_img);
    }
}

int main(){
    bool flag;
    int key = -1;
    cv::Mat frame;
    std::list<KCFTracker> trackers;
    cv::VideoCapture cap("/home/cpk/16s_clip.mp4");
    flag = cap.read(frame);
     cv::imshow("frame",frame);
     Track_Info info(frame);
     cv::setMouseCallback("frame",callback_fun,&info);
     cv::waitKey();
     for(auto iter = info.regions.begin(); iter != info.regions.end(); iter++){
         auto tracker = new KCFTracker(true);
         tracker->init(*iter,info.image);
         trackers.push_back(*tracker);
     }
     cv::Rect rect;
     while(True){
         flag = cap.read(frame);
         key = cv::waitKey(50);
         if (!flag || key != -1){
             break;
         }
         static int i=0;
         i++;
         for(auto iter = trackers.begin(); iter!= trackers.end(); iter++){
             auto start = clock();
             if(i == 15){
             rect = iter->update(frame);

             }
             cv::rectangle(frame,rect,cv::Scalar(0,0,255),2);
             auto end = clock();
         }
         if(i == 15)
             i = 0;
        cv::imshow("frame",frame);
    }
    return 0;
}
