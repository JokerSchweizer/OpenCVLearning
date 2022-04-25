/**
 Universal intrinsics 特征来vectorize代码
 介绍SIMD 内在函数以及如何使用宽寄存器，然后是有关使用宽寄存器的基本操作的教程。
 OpenCVs 通用内在函数提供了对 SIMD 矢量化方法的抽象，并允许用户使用内在函数而无需编写系统特定代码。

 OpenCV Universal Intrinsics 支持以下指令集：

 支持各种类型的128 位寄存器，适用于各种架构，包括
 x86(SSE/SSE2/SSE4.2),
 ARM（NEON），
 PowerPC(VSX),
 MIPS（MSA）。
 x86(AVX2) 支持256 位寄存器
 x86(AVX512) 支持512 位寄存器


寄存器结构=======================================================================
Universal Intrinsics 集将每个寄存器实现为基于特定 SIMD 寄存器的结构。所有类型都包含nlanes枚举，它给出了该类型可以容纳的确切值的数量。

每个寄存器结构都在cv命名空间下。有两种类型的寄存器：
    1.可变大小寄存器：这些结构没有固定大小，它们的确切位长在编译期间根据可用的 SIMD 功能推导出来。因此，nlanes枚举的值是在编译时确定的。
每个结构都遵循以下约定：
  v_[type of value][size of each value in bits]
  v_uint8 保存 8 位无符号整数，v_float32 保存 32 位浮点值。

  v_uint8 a;               // a is a register supporting uint8(char) data
  int n = a.nlanes;        // n holds 32

  不同类型的位数
  uint：	8, 16, 32, 64
  int：	8, 16, 32, 64
  float：	32, 64

    2.恒定大小的寄存器：这些结构具有固定的位大小并保存恒定数量的值。需要知道系统支持哪些 SIMD 指令集并选择兼容的寄存器。仅当需要精确的位长度时才使用这些。
每个结构都遵循约定：
  v_[type of value][size of each value in bits]x[number of values]

  v_int32x8 reg1                       // holds 8 32-bit signed integers.
   v_float64x8 reg2                     // reg2.nlanes = 8


加载和存储操作===================================================================
加载：加载函数允许您将值加载到寄存器中
  构造函数- 声明寄存器结构时，可以提供一个内存地址，寄存器将从该地址获取连续值，或者显式提供多个参数的值（显式多个参数仅适用于常量大小的寄存器）：
  float ptr[32] = {1, 2, 3 ..., 32};   // ptr is a pointer to a contiguous memory block of 32 floats

 // Variable Sized Registers //
 int x = v_float32().nlanes;          // set x as the number of values the register can hold

 v_float32 reg1(ptr);                 // reg1 stores first x values according to the maximum register size available.
 v_float32 reg2(ptr + x);             // reg stores the next x values

 // Constant Sized Registers //
 v_float32x4 reg1(ptr);               // reg1 stores the first 4 floats (1, 2, 3, 4)
 v_float32x4 reg2(ptr + 4);           // reg2 stores the next 4 floats (5, 6, 7, 8)

 // Or we can explicitly write down the values.
 v_float32x4(1, 2, 3, 4);

 加载函数——可以使用加载方法并提供数据的内存地址：
 float ptr[32] = {1, 2, 3, ..., 32};
 v_float32 reg_var;
 reg_var = vx_load(ptr);              // loads values from ptr[0] upto ptr[reg_var.nlanes - 1]

 v_float32x4 reg_128;
 reg_128 = v_load(ptr);               // loads values from ptr[0] upto ptr[3]

 v_float32x8 reg_256;
 reg_256 = v256_load(ptr);            // loads values from ptr[0] upto ptr[7]

 v_float32x16 reg_512;
 reg_512 = v512_load(ptr);            // loads values from ptr[0] upto ptr[15]
 加载函数假定数据未对齐。如果您的数据是对齐的，您可以使用该vx_load_aligned()功能



存储：存储函数允许您将寄存器中的值存储到特定的内存位置。
 要将寄存器中的值存储到内存位置，您可以使用v_store()函数：
 float ptr[4];
 v_store(ptr, reg); // store the first 128 bits(interpreted as 4x32-bit floats) of reg into ptr.
 确保ptr与 register 具有相同的类型。您还可以在执行操作之前将寄存器转换为正确的类型。


运算=============================================================================
    算术：可以按元素对两个寄存器进行加、减、乘和除。寄存器必须具有相同的宽度并保持相同的类型。
    v_float32 a, b;                          // {a1, ..., an}, {b1, ..., bn}
    v_float32 c;
    c = a + b                                // {a1 + b1, ..., an + bn}
    c = a * b;                               // {a1 * b1, ..., an * bn}

    按位逻辑和移位：可以左移或右移寄存器每个元素的位。还可以在两个寄存器元素之间应用按位 &、|、^ 和 ~ 运算符
    v_int32 为；// {a1, ..., an}
    v_int32 al = as << 2; // {a1 << 2, ..., an << 2}
    v_int32 bl = as >> 2; // {a1 >> 2, ..., an >> 2}

    v_int32 a, b;
    v_int32 a_and_b = a & b; // {a1 & b1, ..., an & bn}


    比较运算符：可以使用 <、>、<=、>=、== 和 != 运算符比较两个寄存器之间的值。
    由于每个寄存器都包含多个值，因此不会为这些操作获得一个布尔值。
    相反，对于真值，所有位都转换为 1（0xff 表示 8 位，0xffff 表示 16 位等），而假值则返回转换为零的位。

    // 考虑下面的代码运行在一个 128 位的寄存器
    v_uint8 a;                               // a = {0, 1, 2, ..., 15}
    v_uint8 b;                               // b = {15, 14, 13, ..., 0}

    v_uint8 c = a < b;


    二进制中的前 4 个值

      a = |00000000|00000001|00000010|00000011|
      b = |00001111|00001110|00001101|00001100|
      c = |11111111|11111111|11111111|11111111|

       c 的值并将它们打印为整数，我们将得到 255 表示真值，0 表示假值。

  ---
    在支持 256 位寄存器的计算机中
      v_int32 a;                               // a = {1, 2, 3, 4, 5, 6, 7, 8}
      v_int32 b;                               // b = {8, 7, 6, 5, 4, 3, 2, 1}

      v_int32 c = (a < b);                     // c = {-1, -1, -1, -1, 0, 0, 0, 0}

      真正的值是 0xffffffff，在有符号的 32 位整数表示中等于 -1

最小/最大操作：可以使用v_min()和v_max()函数返回包含两个寄存器的元素最小值或最大值的寄存器：
  v_int32 a;                               // {a1, ..., an}
  v_int32 b;                               // {b1, ..., bn}

  v_int32 mn = v_min(a, b);                // {min(a1, b1), ..., min(an, bn)}
  v_int32 mx = v_max(a, b);                // {max(a1, b1), ..., max(an, bn)}

  比较和最小/最大运算符不适用于 64 位整数。按位移位和逻辑运算符仅适用于整数值。位移位仅适用于 16、32 和 64 位寄存器。

规约和掩盖=======================================================================

规约操作：v_reduce_min()、v_reduce_max()和v_reduce_sum()返回一个值，表示整个寄存器的最小值、最大值、总和
 v_int32 a;                                //  a = {a1, ..., a4}
 int mn = v_reduce_min(a);                 // mn = min(a1, ..., an)
 int sum = v_reduce_sum(a);                // sum = a1 + ... + an

 掩码操作：掩码操作允许我们在宽寄存器中复制条件。这些包括：
 v_check_all() - 返回一个布尔值，如果寄存器中的所有值都小于零，则为真。
 v_check_any() - 返回一个布尔值，如果寄存器中的任何值小于零，则为真。
 v_select() - 返回一个寄存器，它基于掩码混合两个寄存器。

//如果掩码为真（所有位设置为 1），“Res”将包含来自“a”的值，如果掩码为假，则来自“b”的值（所有位设置为 0），
      可以使用比较运算符生成掩码和 v_select 以根据条件获得结果。
      通常将 b 的所有值设置为 0。因此，v_select 将根据掩码给出“a”或 0 的值。
  v_uint8 a;                           // {a1, .., an}
  v_uint8 b;                           // {b1, ..., bn}
  v_int32x4 mask:                      // {0xff, 0, 0, 0xff, ..., 0xff, 0}
  v_uint8 Res = v_select(mask, a, b)   // {a1, b2, b3, a4, ..., an-1, bn}


向量化卷积=======================================================================

一维卷积：标量形式
    void conv1d(Mat src, Mat &dst, Mat kernel)
    {
      int len = src.cols;
      dst = Mat(1, len, CV_8UC1);
      int sz = kernel.cols / 2;
      copyMakeBorder(src, src, 0, 0, sz, sz, BORDER_REPLICATE);
      for (int i = 0; i < len; i++)
      {
          double value = 0;
          for (int k = -sz; k <= sz; k++)
              value += src.ptr<uchar>(0)[i + k + sz] * kernel.ptr<float>(0)[k + sz];
          dst.ptr<uchar>(0)[i] = saturate_cast<uchar>(value);
      }
    }

 一维卷积：向量形式
     void conv1dsimd(Mat src, Mat kernel, float *ans, int row = 0, int rowk = 0, int len = -1)
    {
       if (len == -1)
           len = src.cols;

       //内核是一个浮点数。由于内核的数据类型最大，将 src 转换为 float32，形成src_32。
       Mat src_32, kernel_32;
       const int alpha = 1;
       src.convertTo(src_32, CV_32FC1, alpha);
       //设置好边界
       int ksize = kernel.cols, sz = kernel.cols / 2;
       copyMakeBorder(src_32, src_32, 0, 0, sz, sz, BORDER_REPLICATE);

       //对于内核中的每一列，计算该值与所有长度为的窗口step向量的标量积。将这些值添加到 ans 中已经存储的值中

       int step = v_float32().nlanes;
       //声明一个指向 src_32 和内核的指针，并循环遍历内核元素
       float *sptr = src_32.ptr<float>(row), *kptr = kernel.ptr<float>(rowk);

       for (int k = 0; k < ksize; k++)
       {
         //用当前内核元素加载一个寄存器。一个窗口从0移动到len-step并且它与 kernel_wide 数组的乘积被添加到存储在ans中的值中。将值存储回ans
           v_float32 kernel_wide = vx_setall_f32(kptr[k]);
           int i;
           for (i = 0; i + step < len; i += step)
           {
               v_float32 window = vx_load(sptr + i + k);
               v_float32 sum = vx_load(ans + i) + kernel_wide * window;
               v_store(ans + i, sum);
           }
           //由于长度可能不能被步长整除，直接处理剩余的值。尾值的数量将始终小于步长，不会显着影响性能。将所有值存储到ans中，它是一个浮点指针。也可以直接将它们存储在一个Mat对象中
           for (; i < len; i++)
           {
               *(ans + i) += sptr[i + k]*kptr[k];
           }
       }
    }




    示例:
    kernel: {k1, k2, k3}
    src:           ...|a1|a2|a3|a4|...


    iter1:
    for each idx i in (0, len), 'step' idx at a time
        kernel_wide:          |k1|k1|k1|k1|
        window:               |a0|a1|a2|a3|
        ans:               ...| 0| 0| 0| 0|...
        sum =  ans + window * kernel_wide
            =  |a0 * k1|a1 * k1|a2 * k1|a3 * k1|

    iter2:
        kernel_wide:          |k2|k2|k2|k2|
        window:               |a1|a2|a3|a4|
        ans:               ...|a0 * k1|a1 * k1|a2 * k1|a3 * k1|...
        sum =  ans + window * kernel_wide
            =  |a0 * k1 + a1 * k2|a1 * k1 + a2 * k2|a2 * k1 + a3 * k2|a3 * k1 + a4 * k2|

    iter3:
        kernel_wide:          |k3|k3|k3|k3|
        window:               |a2|a3|a4|a5|
        ans:               ...|a0 * k1 + a1 * k2|a1 * k1 + a2 * k2|a2 * k1 + a3 * k2|a3 * k1 + a4 * k2|...
        sum =  sum + window * kernel_wide
            =  |a0*k1 + a1*k2 + a2*k3|a1*k1 + a2*k2 + a3*k3|a2*k1 + a3*k2 + a4*k3|a3*k1 + a4*k2 + a5*k3|

  二维卷积
  void convolute_simd(Mat src, Mat &dst, Mat kernel)
    {
      //首先初始化变量，并在src矩阵的上方和下方做一个边框。左侧和右侧由一维卷积函数处理
    int rows = src.rows, cols = src.cols;
    int ksize = kernel.rows, sz = ksize / 2;
    dst = Mat(rows, cols, CV_32FC1);
    copyMakeBorder(src, src, sz, sz, 0, 0, BORDER_REPLICATE);
    int step = v_float32().nlanes;

    //对于每一行，计算它上面和下面的行的一维卷积。然后将值添加到dst矩阵
    for (int i = 0; i < rows; i++)
    {
        for (int k = 0; k < ksize; k++)
        {
            float ans[N] = {0};
            conv1dsimd(src, kernel, ans, i + k, k, cols);
            int j;
            for (j = 0; j + step < cols; j += step)
            {
                v_float32 sum = vx_load(&dst.ptr<float>(i)[j]) + vx_load(&ans[j]);
                v_store(&dst.ptr<float>(i)[j], sum);
            }
            for (; j < cols; j++)
                dst.ptr<float>(i)[j] += ans[j];
        }
    }
  //  最终将dst矩阵转换为8 位 unsigned char矩阵
    const int alpha = 1;
    dst.convertTo(dst, CV_8UC1, alpha);
    }

  */
