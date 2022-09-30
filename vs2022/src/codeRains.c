/***********************【程序说明】********************************/
//                文件名：codeRains.c 
//                介绍：win32代码雨程序，致敬帝国，感谢它让我爱上了编程
//                作者：茫静无
//                时间：2022/1/14
//                最后一次修改时间：2022/09/30
/******************************************************************/

//-------------------------------【头文件包含部分】-------------------------------
// 描述：引入必要的头文件
//-------------------------------------------------------------------------------
//windows编程嘛，当然需要包含windows头文件，以便使用windowsAPI函数编程
#include <windows.h> 
#include <stdlib.h> 

//-------------------------------【宏定义部分】------------------------------------
// 描述：定义一些辅助的宏
//--------------------------------------------------------------------------------
//定义两宏表示窗口的大小
#define  WND_WIDTH   900
#define  WND_HEIGHT  700
//雨点个数
#define  RAINS_MAX_NUM   50

//定义表示界面元素的宏

//-------------------------------【全局结构体声明部分】----------------------------
// 描述：声明一些结构体
//--------------------------------------------------------------------------------
typedef struct{
    int x;
    int y;
    int brightness;
}RAINS;

//-------------------------------【WndProc()函数声明】----------------------------
// 描述：下面声明一个窗口过程（回调）函数。当事件发生时，Windows会系统调用该函数，
//我们可以为我们感兴趣的事件写上处理代码。
//--------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );





//定义一个WinMain函数，类似与c语言控制台程序要定义一个main函数来作为入口函数
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    PSTR szCmdLine, int iCmdShow )
{
    /**************************************************************************/
    //Step1:创建一个窗口类，填写窗口的特征
    /**************************************************************************/
    HWND        hwnd;   //定义一个窗口句柄
    MSG         msg;    //定义一个消息结构
    WNDCLASS   wndclass;    //定义一个窗口类
    static TCHAR AppName[] = TEXT("codeRains"); //给我们的窗口类取一个名字
    //给窗口类各字段赋值
    wndclass.style              = CS_HREDRAW | CS_VREDRAW; //设置为窗口大小发生改变时要重新绘制
    wndclass.lpfnWndProc        = WndProc;  //为窗口类绑定窗口过程函数
    wndclass.cbClsExtra         = 0;    //设为0即可，不必深究
    wndclass.cbWndExtra         = 0;    //设为0即可
    wndclass.hInstance          = hInstance;
    //给程序设置一个默认的图标
    
    //也可以为应用程序设定一个自定义的图标,！！！必须要严格的ico格式图标文件，与代码文件放在同一目录下!!!
    wndclass.hIcon              = (HICON) LoadImage( NULL, TEXT("../picture/matrix.ico"), IMAGE_ICON, 
                                                    0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE );
    
    wndclass.hCursor            = LoadCursor( NULL, IDC_ARROW ); //使用默认的鼠标指针
    //选择一个喜欢颜色的画刷，系统会用这种颜色把我们的窗口背景刷成黑色
    wndclass.hbrBackground      = (HBRUSH) GetStockObject( BLACK_BRUSH );    
    wndclass.lpszClassName      = AppName;  //前面已经取好了的名字
    /*****************************************************************************************/


    /*******************************************************************************/
    //Step2:注册窗口类，告诉Windows我们的窗口属性已经设定好了
    /*******************************************************************************/
    if ( !RegisterClass( &wndclass ) )
    {
        // 如果注册失败，输出出错提示信息
        MessageBox( NULL, TEXT("error!!!"), TEXT("codeRains"), MB_ICONERROR );

        return 0;
    }

    //创建窗口
    hwnd = CreateWindow ( AppName,               //窗口类名称
                          TEXT("codeRains"),     //窗口的标题
                          WS_OVERLAPPEDWINDOW,   //默认这样就好了
                          CW_USEDEFAULT,         //窗口出现的x坐标,设为默认的好了
                          CW_USEDEFAULT,         //窗口出现的y坐标
                          WND_WIDTH,             //窗口的宽度
                          WND_HEIGHT,            //窗口的高度
                          NULL,                  //剩余参数，不用深究
                          NULL,
                          hInstance,
                          NULL );

    ShowWindow( hwnd, iCmdShow );//显示窗口，这个时候实实在在可以看到我们的窗口了
    //接下来显示内容
    UpdateWindow( hwnd );
    //消息循环
    while ( GetMessage( &msg, NULL, 0 ,0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    } 

    return msg.wParam;
}



/*******************************************************************************/
//step3:在窗口过程中写主体代码
/*******************************************************************************/

LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    HDC                 mdc,hdc;//定义设备dc，方便绘图
    PAINTSTRUCT              ps;
    static RECT            rect;
    HBITMAP                hBitmap;//定义一个位图句柄
    static RAINS     rains[RAINS_MAX_NUM] = { {0, 0, 0} };            
    int i;
    HPEN            pen;
    HBRUSH          hBrush;
    
    switch (message)
    {
        
        case WM_CREATE:
            GetClientRect( hwnd, &rect );
            //随机生成雨点的坐标位置和初始亮度
            for (i=0; i<RAINS_MAX_NUM; i++)
            {
                rains[i].x = rand() % 890;
                rains[i].y = rand() % 600;
                rains[i].brightness = rand() % 255;
            }
            SetTimer( hwnd, 1, 700, NULL );
            break;

        case WM_TIMER:
            
            for (i=0; i<RAINS_MAX_NUM; i++)
            {
                //雨点没有到达窗口底部，就继续下落，亮度继续增强
                if ( rains[i].y < 650 || rains[i].brightness < 255 )
                {
                    rains[i].y += 15;
                    rains[i].brightness += 20;
                }
                else//雨点消失，重新生成一个
                { 
                    rains[i].x = rand() % 890;
                    rains[i].y = rand() % 600;
                    rains[i].brightness = rand() % 255;
                }
                
            }
            //间接发送WM_PAINT消息
            //第三个参数为FALSE表示重绘时不擦除原来显示的内容
            InvalidateRect( hwnd, NULL, FALSE );
            break;
        case WM_PAINT:
            hdc = BeginPaint( hwnd, &ps );//绘图从此处开始
            pen = CreatePen( PS_INSIDEFRAME, 1, RGB( 255, 0, 0 ) );//创建红色画笔
            SelectObject( hdc, pen );//将画笔选入当前设备
            hBrush = CreateSolidBrush( RGB( 0, 255, 0 ) );//创建绿色画刷
            SelectObject( hdc, hBrush);//将画刷选入当前设备
            //准备显示一张海报
            hBitmap = (HBITMAP)LoadImage( NULL,
                                          TEXT("../picture/matrix.bmp"),//海报文件名
                                          IMAGE_BITMAP,//图片类型
                                          420,//图片的长度/像素
                                          623,//图片的宽度
                                          LR_LOADFROMFILE );//从本地文件导入
            mdc = CreateCompatibleDC( hdc );
            SelectObject ( mdc, hBitmap );
            BitBlt( hdc, 250, 0, 420, 623, mdc, 0, 0, SRCCOPY );//把海报贴图在屏幕正中央
            //绘制数字
            for (i=0; i<RAINS_MAX_NUM; i++)
            {
                pen = CreatePen( PS_INSIDEFRAME, 1, RGB( 0, rains[i].brightness, 0 ) );
                SelectObject( hdc, pen );
                hBrush = CreateSolidBrush( RGB(0, rains[i].brightness, 0) );
                SelectObject( hdc, hBrush );
                //偷个懒，用绿色矩形代替字符，主要晚上有作业，没时间改进<*,*>哈哈
                Rectangle( hdc, rains[i].x, rains[i].y, rains[i].x + 5, rains[i].y + 5 );
            }
            DeleteDC( mdc );
            DeleteObject( hBitmap );
            EndPaint( hwnd, &ps );//绘图结束
            break;

     
        case WM_DESTROY://在此处进行善后工作
            KillTimer( hwnd, 1 );
            PostQuitMessage( 0 );//保证程序彻底退出
            break;
        
    }
    return DefWindowProc( hwnd, message, wParam, lParam );
}




