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

Mat addColorMatToBwMatWithMask(Mat colorMat, Mat bwMat, Mat maskMat, Vec3b th){
    int rowsColorMat = colorMat.rows;
    int colsColorMat = colorMat.cols;
    int rowsBwMat = bwMat.rows;
    int colsBwMat = bwMat.cols;
    int rowsMaskMat = maskMat.rows;
    int colsMaskMat = maskMat.cols;

    Mat newMatEnsamble = bwMat.clone();
    if((rowsColorMat == rowsBwMat) && (rowsColorMat == rowsMaskMat) && (colsColorMat == colsBwMat) && (colsColorMat == colsMaskMat)){
        for(int index_y = 0;index_y < rowsMaskMat; index_y++){
            for(int index_x = 0;index_x < colsMaskMat; index_x++){
                Vec3b valMask = maskMat.at<Vec3b>(Point(index_x,index_y));
                if((valMask[0] != valMask[1] && valMask[1] != valMask[2]) && valMask[2] > 200){
                    Vec3b valColorMat = colorMat.at<Vec3b>(Point(index_x,index_y));
                    valColorMat[2] = (valColorMat[2] + 250)/2;
                    valColorMat[1] = (valColorMat[1] + valColorMat[1])/2;
                    valColorMat[0] = (valColorMat[0] + valColorMat[0])/2;
                    newMatEnsamble.at<Vec3b>(Point(index_x,index_y)) = valColorMat;
                /*
                    val_0 = newMatEnsamble.at<Vec3b>(Point(index_x-1,index_y-1))
                    val_1 = newMatEnsamble.at<Vec3b>(Point(index_x,index_y-1))
                    val_2 = newMatEnsamble.at<Vec3b>(Point(index_x+1,index_y-1))
                    val_3 = newMatEnsamble.at<Vec3b>(Point(index_x-1,index_y))
                    val_4 = newMatEnsamble.at<Vec3b>(Point(index_x,index_y))
                    val_5 = newMatEnsamble.at<Vec3b>(Point(index_x+1,index_y))
                    val_6 = newMatEnsamble.at<Vec3b>(Point(index_x-1,index_y+1))
                    val_7 = newMatEnsamble.at<Vec3b>(Point(index_x,index_y+1))
                    val_8 = newMatEnsamble.at<Vec3b>(Point(index_x+1,index_y+1))
                    newMatEnsamble.at<Vec3b>(Point(index_x,index_y)) = (val_0 + val_1 + val_2 + val_3 + val_4 + val_5 + val_6 + val_7 + val8)/9
                */
                }
            }
        }
        cout << "------- addColorMatToBwMatWithMask ------- end" << endl;
        return newMatEnsamble;
    }
    else{
        cout << "----c--- addTwoMat3Channel ------- end" << endl;
        return Mat(0,0,0);
    }
}





// 16 in x sono 90 cm
// 23 in y sono quindi 130

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);


    QString imagePath = "C:\\Users\\Fabio Roncato\\Documents\\Qt\\12_09_2019_rebif\\brain-damage_16.png";
    QString imagePathToSave = "C:\\Users\\Fabio Roncato\\Documents\\Qt\\12_09_2019_rebif\\brain-damage_16_test.png";
    Mat image = imread(imagePath.toStdString(),CV_LOAD_IMAGE_COLOR);
    for(int index_y = 0;index_y < image.rows; index_y++){
        for(int index_x = 0;index_x < image.cols; index_x++){
            Vec3b valColorMat = image.at<Vec3b>(Point(index_x,index_y));
            if (valColorMat[0] == 255 && valColorMat[1] == 255 && valColorMat[2] == 255)
                image.at<Vec3b>(Point(index_x,index_y)) = Vec3b(0,0,0);
            if (valColorMat[0] == valColorMat[1] && valColorMat[1] == valColorMat[2])
                image.at<Vec3b>(Point(index_x,index_y)) = Vec3b(0,0,0);
            else
                image.at<Vec3b>(Point(index_x,index_y)) = Vec3b(255,255,255);
        }
    }
    imwrite(imagePathToSave.toStdString(), image);



    cout << "finish !!!" << endl;
    waitKey(10);
    return a.exec();
}


/*

#define ALTERNATIVE
    // DIM IMG 12124 in x - 14659 in y
    QCoreApplication a(argc, argv);
    ///////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////// Settings parameters ///////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////
    const int images_in_x = 23;
    const int images_in_y = 16;
    const int imageDimension_y = 915; // dimension images in "C:\Users\Fabio Roncato\Documents\images_rebif\new_rebif" imageDimension x imageDimension
    const int imageDimension_x = 526;
    const int border_pixel = 2; //imageDimension_x/2;
    const int additional_border = 526;
    QDate startingDay(2010, 5, 17);
    QDate endingDay(2011, 5, 18);
    int whereXBibImage = 1080;
    int whereYBibImage = 8720;
    QFile file("C:\\Users\\Fabio Roncato\\Documents\\Qt\\24_02_2019_rebif\\date_primo_anno.txt");
    QString pathImageFrontInjection = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\injectionSite_front_";
    QString pathImageBackInjection = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\injectionSite_back_";
    QString imageNoInjection = "C:\\Users\\Fabio Roncato\\Documents\\images_rebif\\injectionSite_0.png";
    QString savePathBigInjectionImage = "C:\\Users\\Fabio Roncato\\Documents\\Qt\\24_02_2019_rebif\\rebif_color_done.jpg";
    QString imageSentence = "C:\\Users\\Fabio Roncato\\Documents\\rebif\\ScrittaDaPreparare_7_bis.png";
    QString imageSentencePlusBigImageDone = "C:\\Users\\Fabio Roncato\\Documents\\Qt\\24_02_2019_rebif\\out.jpg";
    QString imagePhotoToAdd = "C:\\Users\\Fabio Roncato\\Documents\\rebif\\IMG-4632_tris_bw.jpg"; //12124 in x - 14659 in y
    QString maskImagePhotoToAdd = "C:\\Users\\Fabio Roncato\\Documents\\rebif\\IMG-4632_tris_mask.jpg";
    QString colorImagePhotoToAdd = "C:\\Users\\Fabio Roncato\\Documents\\rebif\\IMG-4632_tris_color.jpg";
    QString imageFinal = "C:\\Users\\Fabio Roncato\\Documents\\Qt\\24_02_2019_rebif\\out2.jpg";
    QString imageFinalWithBorder = "C:\\Users\\Fabio Roncato\\Documents\\Qt\\24_02_2019_rebif\\out3.jpg";




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

    // add the two image ( big image injection + sentence image) and save it
    Mat matSentence = imread(imageSentence.toStdString());
    //Mat outputImage1 = addTwo3ChannelMat(bigImageAllInjection, matSentence, Vec3b(255,255,255));
    Mat outputImage1 = addSmallImageToBigMat(bigImageAllInjection, matSentence, Vec3b(255,255,255), whereXBibImage, whereYBibImage);
    imwrite(imageSentencePlusBigImageDone.toStdString() , outputImage1 );

#ifdef ALTERNATIVE
    Mat imagePhoto = imread(imagePhotoToAdd.toStdString(),CV_LOAD_IMAGE_COLOR);
    Mat colorImagePhoto = imread(colorImagePhotoToAdd.toStdString(),CV_LOAD_IMAGE_COLOR);
    Mat maskImagePhoto = imread(maskImagePhotoToAdd.toStdString());
    Mat imagePhotoToAddNew = addColorMatToBwMatWithMask(colorImagePhoto, imagePhoto, maskImagePhoto, Vec3b(0,0,255));
    Mat outputImage2 = addTwo3ChannelMat(imagePhotoToAddNew, outputImage1, Vec3b(255,255,255));
    imwrite(imageFinal.toStdString() , outputImage2 );
#else
    // add the previous image and the photo image( big image injection + sentence image  + photo image) and save it
    Mat imagePhoto = imread(imagePhotoToAdd.toStdString(),CV_LOAD_IMAGE_COLOR);
    Mat outputImage2 = addTwo3ChannelMat(imagePhoto, outputImage1, Vec3b(255,255,255));
    imwrite(imageFinal.toStdString() , outputImage2 );
#endif

    // add white border left-right, up-down
    outputImage2.copyTo(bigImageAllInjectionWithAdditionalBorder(cv::Rect(additional_border,additional_border,outputImage2.cols, outputImage2.rows)));
    imwrite(imageFinalWithBorder.toStdString() , bigImageAllInjectionWithAdditionalBorder );


    cout << "finish !!!" << endl;
    waitKey(10);
    return a.exec();
}
*/


