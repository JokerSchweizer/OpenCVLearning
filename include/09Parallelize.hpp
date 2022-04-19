//
// Created by welsh on 2022/4/19.
//

void conv_seq(Mat src, Mat &dst, Mat kernel)
{
    int rows = src.rows, cols = src.cols;
    dst = Mat(rows, cols, src.type());  //创建同等大小的图像
    // Taking care of edge values
    // Make border = kernel.rows / 2;
    int sz = kernel.rows / 2;
    copyMakeBorder(src, src, sz, sz, sz, sz, BORDER_REPLICATE); //宽展图像的边界，这样就都能用kernel来卷积到了
    for (int i = 0; i < rows; i++)
    {
        uchar *dptr = dst.ptr(i); //取当前行
        for (int j = 0; j < cols; j++) //对应列
        {
            double value = 0;
            for (int k = -sz; k <= sz; k++)
            {
                // slightly faster results when we create a ptr due to more efficient memory access.
                uchar *sptr = src.ptr(i + sz + k);
                for (int l = -sz; l <= sz; l++)
                {
                    value += kernel.ptr<double>(k + sz)[l + sz] * sptr[j + sz + l];
                }
            }
            dptr[j] = saturate_cast<uchar>(value); //saturate_cast这个保证数值在0~255 区间
        }
    }
}

//////////////////////////////////并行版本///////////////////////////////////////////////

//OpenCV cv::parallel_for_框架自动决定如何有效地拆分计算并为我们完成大部分工
//声明一个继承自cv::ParallelLoopBody的自定义类并覆盖virtual void operator ()(const cv::Range& range) const.

//调用方法
parallelConvolution obj(src, dst, kernel);
parallel_for_(Range(0, rows * cols), obj);




class parallelConvolution : public ParallelLoopBody
{
private:
    Mat m_src, &m_dst;
    Mat m_kernel;
    int sz;
public:
    parallelConvolution(Mat src, Mat &dst, Mat kernel)
            : m_src(src), m_dst(dst), m_kernel(kernel)
    {
        sz = kernel.rows / 2;
    }
    virtual void operator()(const Range &range) const CV_OVERRIDE
    {
        for (int r = range.start; r < range.end; r++)
        {
            int i = r / m_src.cols, j = r % m_src.cols;
            double value = 0;
            for (int k = -sz; k <= sz; k++)
            {
                uchar *sptr = m_src.ptr(i + sz + k);
                for (int l = -sz; l <= sz; l++)
                {
                    value += m_kernel.ptr<double>(k + sz)[l + sz] * sptr[j + sz + l];
                }
            }
            m_dst.ptr(i)[j] = saturate_cast<uchar>(value);
        }
    }
};


/**
 * 可以使用：cv::setNumThreads。
 * 还可以使用cv::parallel_for_中的 nstripes 参数指定拆分次数
 * 例如，如果您的处理器有 4 个线程，则设置cv::setNumThreads(2)或设置nstripes=2应与默认情况下相同，它将使用所有可用的处理器线程，但只会将工作负载拆分到两个线程上。


 */




 //C++ 11 标准允许通过去掉parallelConvolution类并用 lambda 表达式替换它来简化并行实现：

    parallel_for_ (Range(0, rows * cols), [&]( const Range &range)
    {
        for ( int r = range.start; r < range.end; r++)
        {
            int i = r / cols, j = r % cols;
            双值 = 0;
            for (int k = -sz; k <= sz; k++)
            {
                uchar *sptr = src.ptr(i + sz + k);
                for (int l = -sz; l <= sz; l++)
                    {
                    值 += kernel.ptr<double>(k + sz)[l + sz] * sptr[j + sz + l];
                    }
            }
            dst.ptr(i)[j] = saturate_cast <uchar>(值);
        }
    });