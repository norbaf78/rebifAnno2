#include <QCoreApplication>
#include <QDir>
#include <QTextStream>
#include <QDirIterator>
#include <QTime>
#include <QString>
#include <iostream>
#include <numeric>
#include <opencv2/opencv.hpp>
#include <QFile>
#include <QDate>
#include <time.h>

using namespace std;
using namespace cv;


Mat addTwo3ChannelMat(Mat matImage, Mat matToAdd, Vec3b th){
    cout << "------- addTwoMat3Channel ------- start" << endl;
    int rows = matImage.rows; // is equal in matMask
    int cols = matImage.cols; // is equal in matMask
    int type = matImage.type(); // is equal in matMask
    Mat newMatEnsamble = matImage.clone();
    cout << "matImage type: " + matImage.type() << endl;
    cout << "matToAdd type: " + matToAdd.type() << endl;
    if((matImage.rows == matToAdd.rows) && (matImage.cols == matToAdd.cols)){  // the dimension of the two images is the same
        if((matImage.channels() == 3) && (matToAdd.channels() == 3) && (matImage.type() == matToAdd.type())){ // the type of the two images is the same and both have 3 channel
            for(int index_y = 0;index_y < rows; index_y++){
                for(int index_x = 0;index_x < cols; index_x++){
                    Vec3b val = matToAdd.at<Vec3b>(Point(index_x,index_y)); // get the current point value in matToAdd
                    if (val != th){  // if the pixel value in matToAdd is different from the setted value, set the mask value
                        newMatEnsamble.at<Vec3b>(Point(index_x,index_y)) = val;
                    }
                }
            }
            cout << "------- addTwoMat3Channel ------- end" << endl;
            return newMatEnsamble;
        }
        else{
            cout << "----b--- addTwoMat3Channel ------- end" << endl;
            return Mat(1,1,0);
        }
    }
    else{
        cout << "----c--- addTwoMat3Channel ------- end" << endl;
        return Mat(0,0,0);
    }
}



Mat addSmallImageToBigMat(Mat bigMatImage, Mat smallMatToAdd, Vec3b th, int startFromBigImage_X, int startFromBigImage_Y){
    cout << "------- addSmallImageToBigMat ------- start" << endl;
    int rowsBigMat = bigMatImage.rows;
    int colsBigMat = bigMatImage.cols;
    int typeBigMat = bigMatImage.type();
    int rowsSmallMat = smallMatToAdd.rows;
    int colsSmallMat = smallMatToAdd.cols;
    int typeSmallMat = smallMatToAdd.type();
    Mat newMatEnsamble = bigMatImage.clone();
    cout << "bigMatImage type: " + bigMatImage.type() << endl;
    cout << "smallMatToAdd type: " + smallMatToAdd.type() << endl;
    for(int index_y = startFromBigImage_Y;index_y < startFromBigImage_Y + rowsSmallMat; index_y++){
        for(int index_x = startFromBigImage_X;index_x < startFromBigImage_X + colsSmallMat; index_x++){
            Vec3b val = smallMatToAdd.at<Vec3b>(Point((index_x-startFromBigImage_X),(index_y-startFromBigImage_Y))); // get the current point value in matToAdd
            if (val != th){  // if the pixel value in matToAdd is different from the setted value, set the mask value
                newMatEnsamble.at<Vec3b>(Point(index_x,index_y)) = val;
            }
         }
    }
    cout << "------- addSmallImageToBigMat ------- end" << endl;
    return newMatEnsamble;
}






// 16 in x sono 90 cm
// 23 in y sono quindi 130

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ///////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////// Settings parameters ///////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////
    const int images_in_x = 23;
    const int images_in_y = 16;
    const int imageDimension_y = 915; // dimension images in of the place in the body where injection has been made
    const int imageDimension_x = 526;
    const int border_pixel = 2; //imageDimension_x/2;
    const int additional_border = 526*2/3;
    QDate startingDay(2011, 5, 17);
    QDate endingDay(2012, 5, 18);
    int whereXBibImage = 1080;
    int whereYBibImage = 3720;
    QFile file("C:\\Users\\Fabio Roncato\\Documents\\Qt\\12_09_2019_rebif\\date_secondo_anno.txt");
    QString pathImageFrontInjection = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\injectionSite_front_";
    QString pathImageBackInjection = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\injectionSite_back_";
    QString imageNoInjection = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\injectionSite_0.png";
    QString savePathBigInjectionImage = "C:\\Users\\Fabio Roncato\\Documents\\Qt\\12_09_2019_rebif\\rebif_color_done.jpg";
    QString imageBrain = "C:\\Users\\Fabio Roncato\\Documents\\Qt\\12_09_2019_rebif\\brainWithDots2_ok128_tris.png";
    QString imageBrainPlusBigImageDone = "C:\\Users\\Fabio Roncato\\Documents\\Qt\\12_09_2019_rebif\\out_brain.jpg";
    QString imagePhotoToAdd = "C:\\Users\\Fabio Roncato\\Documents\\Qt\\12_09_2019_rebif\\big_risonance.jpg"; //12124 in x - 14659 in y
    QString imageFinal = "C:\\Users\\Fabio Roncato\\Documents\\Qt\\12_09_2019_rebif\\out2_brain.jpg";
    QString imageFinalWithBorder = "C:\\Users\\Fabio Roncato\\Documents\\Qt\\12_09_2019_rebif\\out3_brain.jpg";

    QString pathBackgroundImages_1 = "C:\\Users\\Fabio Roncato\\Documents\\Risonanze\\IMG_anno2\\20110503";
    int numberImages_1 = 163;
    QString pathBackgroundImages_2 = "C:\\Users\\Fabio Roncato\\Documents\\Risonanze\\IMG_anno2\\20120619";
    int numberImages_2 = 187;
    float zoomToApplay = 4;
    QString savePathBigRisonanceImage ="C:\\Users\\Fabio Roncato\\Documents\\Qt\\12_09_2019_rebif\\big_risonance.jpg";
    int number_of_iteration = 3000;


    ///////////////////////////////////////////////////////////////////////////////////
    //
    //
    ///////////////////////////////////////////////////////////////////////////////////
    Mat bigImageRisonance(2*border_pixel+(images_in_y*imageDimension_y)+(images_in_y-1)*1, 2*border_pixel+(images_in_x*imageDimension_x)+(images_in_x-1)*1, CV_8UC3, Scalar(9,9,9));
    int bigImageRisonanceX = bigImageRisonance.cols;
    int bigImageRisonanceY = bigImageRisonance.rows;
    srand (time(NULL));

    for(int i=0;i<number_of_iteration;i++){
        int imageNumber_1 = (rand() % numberImages_1) +1;
        QString current_image_path_1 = pathBackgroundImages_1 + "\\" + QString::number(imageNumber_1) + ".jpeg";
        cout << current_image_path_1.toStdString() << endl;
        int imageNumber_2 = (rand() % numberImages_2) +1;
        QString current_image_path_2 = pathBackgroundImages_2 + "\\" + QString::number(imageNumber_2) + ".jpeg";
        cout << current_image_path_2.toStdString() << endl;
        //image will be selected based on the position
        Mat currentImageRisonance_1 = imread(current_image_path_1.toStdString(),CV_LOAD_IMAGE_COLOR);
        Mat currentImageRisonance_2 = imread(current_image_path_2.toStdString(),CV_LOAD_IMAGE_COLOR);
        Mat currentImageRisonance;
        bool from_1;
        if(currentImageRisonance_1.data && currentImageRisonance_2.data){ // both the images selected are available
            int whereToWriteImageRisonanceX = (rand() % (bigImageRisonanceX-(currentImageRisonance_1.cols * int(zoomToApplay))));
            int whereToWriteImageRisonanceY = (rand() % (bigImageRisonanceY-int((currentImageRisonance_1.rows * int(zoomToApplay)))));
            cout << whereToWriteImageRisonanceX << ":" << whereToWriteImageRisonanceY << endl;
            if(whereToWriteImageRisonanceY < (bigImageRisonance.rows/2 - currentImageRisonance_1.rows/2)-bigImageRisonance.rows/8){
                currentImageRisonance = currentImageRisonance_1;
                from_1=true;
            }
            else if(whereToWriteImageRisonanceY > (bigImageRisonance.rows/2 - currentImageRisonance_1.rows/2)+bigImageRisonance.rows/8){
                currentImageRisonance = currentImageRisonance_2;
                from_1=false;
            }
            else{
                srand(time(0));
                int randomval = rand() % 2;
                if(randomval==0){
                    currentImageRisonance = currentImageRisonance_1;
                    from_1=true;
                }
                else{
                    currentImageRisonance = currentImageRisonance_2;
                    from_1=false;
                }
            }
            Mat currentImageRisonanceResized;
            cv::resize(currentImageRisonance,currentImageRisonanceResized, Size(), zoomToApplay, zoomToApplay,CV_INTER_AREA );
            // this is optional. Before add the image is possible write sromething
//            if(from_1)
//                putText(currentImageRisonanceResized, "**", Point(222,222), FONT_HERSHEY_TRIPLEX , 8.0, CV_RGB(255,255,0), 10.0 );
//            else
//                putText(currentImageRisonanceResized, "**", Point(222,222), FONT_HERSHEY_TRIPLEX , 8.0, CV_RGB(0,255,255), 10.0 );
            // insert the image
            currentImageRisonanceResized.copyTo(bigImageRisonance(cv::Rect(whereToWriteImageRisonanceX,whereToWriteImageRisonanceY,currentImageRisonanceResized.cols, currentImageRisonanceResized.rows)));
        }
    }

/*
    for(int i=0;i<number_of_iteration;i++){
        int imageNumber_1 = (rand() % numberImages_1) +1;
        QString current_image_path = pathBackgroundImages_1 + "\\" + QString::number(imageNumber_1) + ".jpeg";
        cout << current_image_path.toStdString() << endl;
        //image has been selected
        Mat currentImageRisonance = imread(current_image_path.toStdString(),CV_LOAD_IMAGE_COLOR);
        if(currentImageRisonance.data){
            Mat currentImageRisonanceResized;
            cv::resize(currentImageRisonance,currentImageRisonanceResized, Size(), zoomToApplay, zoomToApplay,CV_INTER_AREA );

            int whereToWriteImageRisonanceX = (rand() % (bigImageRisonanceX-currentImageRisonanceResized.cols));
            int whereToWriteImageRisonanceY = (rand() % (bigImageRisonanceY/2-currentImageRisonanceResized.rows/2));
            cout << whereToWriteImageRisonanceX << ":" << whereToWriteImageRisonanceY << endl;

            // this is optional. Before add the image is possible write sromething
            //putText(currentImageBody, dayDate.toStdString(), Point(60,imageDimension_y-110), FONT_HERSHEY_TRIPLEX , 2.0, CV_RGB(0,0,0), 4.0 );
            // insert the image
            currentImageRisonanceResized.copyTo(bigImageRisonance(cv::Rect(whereToWriteImageRisonanceX,whereToWriteImageRisonanceY,currentImageRisonanceResized.cols, currentImageRisonanceResized.rows)));
        }
    }

    for(int i=0;i<number_of_iteration;i++){
        int imageNumber_2 = (rand() % numberImages_2) +1;
        QString current_image_path = pathBackgroundImages_2 + "\\" + QString::number(imageNumber_2) + ".jpeg";
        cout << current_image_path.toStdString() << endl;
        //image has been selected
        Mat currentImageRisonance = imread(current_image_path.toStdString(),CV_LOAD_IMAGE_COLOR);
        if(currentImageRisonance.data){
            Mat currentImageRisonanceResized;
            cv::resize(currentImageRisonance,currentImageRisonanceResized, Size(), zoomToApplay, zoomToApplay,CV_INTER_AREA );

            int whereToWriteImageRisonanceX = (rand() % (bigImageRisonanceX-currentImageRisonanceResized.cols));
            int whereToWriteImageRisonanceY = bigImageRisonanceY/2-currentImageRisonanceResized.rows/2 + (rand() % (bigImageRisonanceY/2-currentImageRisonanceResized.rows/2));
            cout << whereToWriteImageRisonanceX << ":" << whereToWriteImageRisonanceY << endl;

            // this is optional. Before add the image is possible write sromething
            //putText(currentImageBody, dayDate.toStdString(), Point(60,imageDimension_y-110), FONT_HERSHEY_TRIPLEX , 2.0, CV_RGB(0,0,0), 4.0 );
            // insert the image
            currentImageRisonanceResized.copyTo(bigImageRisonance(cv::Rect(whereToWriteImageRisonanceX,whereToWriteImageRisonanceY,currentImageRisonanceResized.cols, currentImageRisonanceResized.rows)));
        }
    }

*/

    bigImageRisonance = ~bigImageRisonance;
    imwrite(savePathBigRisonanceImage.toStdString(), bigImageRisonance);
    cout << "Image: " << savePathBigRisonanceImage.toStdString() << endl;
    cout << "finish this step!!!" << endl;



    ///////////////////////////////////////////////////////////////////////////////////
    // open file with information of date, injection yes or no, point of injection (if injection has been done)
    // and read the data from the file and create three QStringList with those informations
    ///////////////////////////////////////////////////////////////////////////////////
    if(!file.open(QFile::ReadOnly|QFile::Text))
        cout << "No file found" << endl;

    QStringList data;
    QStringList iniezione;
    QStringList posizione;
    while ( !file.atEnd() ) {
      QString line = file.readLine();   // read the line
      QStringList list = line.split(" "); // split the line
      QString dateString = list[0]; // read the date
      QString yesOrNo = list[1]; // read yes or no (if the injection has been done or not)
      QString position = list[2]; // read the position (in case the injection has not been done the value will been 0)
      data.append(dateString); // the value are append for the data in "data"
      iniezione.append(yesOrNo); // the value are append for the injection in "iniezione"
      posizione.append(position); // the value are append for the injection position in "posizione"
    }
    cout << endl;
    cout << "data elements: " << data.count() << endl;
    cout << "iniezione elements: " << iniezione.count() << endl;
    cout << "posizione elements: " << posizione.count() << endl;

    // read number of days in the period and number of injection have been done
    int countInjectionDone=0;
    for(int i=0;i< data.count(); i++){
        if(iniezione[i].compare("yes") == 0)
            countInjectionDone++;
    }
    cout << "Injection done: " << countInjectionDone << endl;
    cout << "Total days: " << data.count() << endl;



    ////////////////////////////////////////////////////////////////////////////////////
    /// create an image containing all the images for all the days (imageDimension_x and imageDimension_y are the dimension of the small images used)
    /// and fill the new big image created with the small images available with front, back and no injection
    ////////////////////////////////////////////////////////////////////////////////////
    Mat bigImageAllInjection(2*border_pixel+(images_in_y*imageDimension_y)+(images_in_y-1)*1, 2*border_pixel+(images_in_x*imageDimension_x)+(images_in_x-1)*1, CV_8UC3, Scalar(0,0,0));
    Mat bigImageAllInjectionWithAdditionalBorder(2*additional_border + 2*border_pixel+(images_in_y*imageDimension_y)+(images_in_y-1)*1, 2*additional_border + 2*border_pixel+(images_in_x*imageDimension_x)+(images_in_x-1)*1, CV_8UC3, Scalar(180,180,180));

    // fill the image
    QString filenameCurrentImageBody;
    QString dayDate;
    QString position;
    int index_x=0, index_y=0;
    for(int i=0;i< data.count(); i++){ // for all the date (for all the days)
        // generating which image(different type depending if injection done and where) take
        dayDate = data[i];
        if(iniezione[i].compare("yes") == 0){ // iniezione effettuata
            position = posizione[i];
            if(position.toInt() != 0 && position.toInt()< 19) // effettuata frontalmente
                filenameCurrentImageBody = pathImageFrontInjection + position + ".png";
            else if(position.toInt() != 0 && position.toInt()>= 19) // effettuata posteriormente
                filenameCurrentImageBody = pathImageBackInjection + position + ".png";
        }
        else{ // no injection
            position="0";
            filenameCurrentImageBody = imageNoInjection;
        }
        //image has been selected and date injection will be added
        Mat currentImageBody = imread(filenameCurrentImageBody.toStdString(),CV_LOAD_IMAGE_COLOR);
        cv::threshold(currentImageBody,currentImageBody,254,255,cv::THRESH_BINARY);
        if(position.toInt() != 0){
            putText(currentImageBody, dayDate.toStdString(), Point(60,imageDimension_y-110), FONT_HERSHEY_TRIPLEX , 2.0, CV_RGB(0,0,0), 4.0 );
        }
        // insert the small image into the big one created before
        currentImageBody.copyTo(bigImageAllInjection(cv::Rect(border_pixel+index_x*(imageDimension_x+1),border_pixel+index_y*(imageDimension_y+1),imageDimension_x, imageDimension_y)));
        index_x++;
        if(index_x > images_in_x-1){
            index_y++;
            index_x=0;
        }
    }
    imwrite(savePathBigInjectionImage.toStdString(), bigImageAllInjection);
    cout << "Image: " << savePathBigInjectionImage.toStdString() << endl;

    // add the two image ( big image injection + sentence image) and save it
    Mat matBrain = imread(imageBrain.toStdString());
    whereXBibImage = bigImageAllInjection.cols/2 - matBrain.cols/2;
    whereYBibImage = bigImageAllInjection.rows/2 - matBrain.rows/2;
    Mat outputImage1 = addSmallImageToBigMat(bigImageAllInjection, matBrain, Vec3b(255,255,255), whereXBibImage, whereYBibImage);
    imwrite(imageBrainPlusBigImageDone.toStdString() , outputImage1 );

    Mat imagePhoto = imread(imagePhotoToAdd.toStdString(),CV_LOAD_IMAGE_COLOR);
    Mat outputImage2 = addTwo3ChannelMat(imagePhoto, outputImage1, Vec3b(255,255,255));
    imwrite(imageFinal.toStdString() , outputImage2 );

    // add white border left-right, up-down
    outputImage2.copyTo(bigImageAllInjectionWithAdditionalBorder(cv::Rect(additional_border,additional_border,outputImage2.cols, outputImage2.rows)));
    imwrite(imageFinalWithBorder.toStdString() , bigImageAllInjectionWithAdditionalBorder );

    cout << "finish !!!" << endl;
    waitKey(10);
    return a.exec();
}


