//
// Created by welsh on 2022/4/3.
//

/**
 * 时间测量
 *       double t = (double)getTickCount();
 *              do something ...
 *           t = ((double)getTickCount() - t)/getTickFrequency();
 *           cout << "Times passed in seconds: " << t << endl;
 *
 *
 *
 *      通道的顺序是相反的：BGR 而不是 RGB。
 *
 *      OpenCV提供了修改图像值的功能，无需编写图像的扫描逻辑。我们使用核心模块的cv::LUT()函数。
 *        Mat lookUpTable(1, 256, CV_8U);
          uchar* p = lookUpTable.ptr();
          for( int i = 0; i < 256; ++i)
                p[i] = table[i];

          LUT(I, lookUpTable, J);

 *
*/




/******************************************************************************
        Mat& ScanImageAndReduceC(Mat& I, const uchar* const table)
        {
            // accept only char type matrices
            CV_Assert(I.depth() == CV_8U);
            int channels = I.channels();
            int nRows = I.rows;
            int nCols = I.cols * channels;
            if (I.isContinuous())
            {
                nCols *= nRows;
                nRows = 1;
            }
            int i,j;
            uchar* p;
            for( i = 0; i < nRows; ++i)
            {
                p = I.ptr<uchar>(i);
                for ( j = 0; j < nCols; ++j)
                {
                    p[j] = table[p[j]];
                }
            }
            return I;
        }



       ////////////////////////////////////////////////////////

        Mat& ScanImageAndReduceIterator(Mat& I, const uchar* const table)
        {
            // accept only char type matrices
            CV_Assert(I.depth() == CV_8U);
            const int channels = I.channels();
            switch(channels)
            {
            case 1:
                {
                    MatIterator_<uchar> it, end;
                    for( it = I.begin<uchar>(), end = I.end<uchar>(); it != end; ++it)
                        *it = table[*it];
                    break;
                }
            case 3:
                {
                    MatIterator_<Vec3b> it, end;
                    for( it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; ++it)
                    {
                        (*it)[0] = table[(*it)[0]];
                        (*it)[1] = table[(*it)[1]];
                        (*it)[2] = table[(*it)[2]];
                    }
                }
            }
            return I;
        }
        /////////////////////////////////////////////////////////////////////////////////////////

        Mat& ScanImageAndReduceRandomAccess(Mat& I, const uchar* const table)
        {
            // accept only char type matrices
            CV_Assert(I.depth() == CV_8U);
            const int channels = I.channels();
            switch(channels)
            {
            case 1:
                {
                    for( int i = 0; i < I.rows; ++i)
                        for( int j = 0; j < I.cols; ++j )
                            I.at<uchar>(i,j) = table[I.at<uchar>(i,j)];
                    break;
                }
            case 3:
                {
                 Mat_<Vec3b> _I = I;
                 for( int i = 0; i < I.rows; ++i)
                    for( int j = 0; j < I.cols; ++j )
                       {
                           _I(i,j)[0] = table[_I(i,j)[0]];
                           _I(i,j)[1] = table[_I(i,j)[1]];
                           _I(i,j)[2] = table[_I(i,j)[2]];
                    }
                 I = _I;
                 break;
                }
            }
            return I;
        }

 */