/***********************������˵����********************************/
//                �ļ�����codeRains.c 
//                ���ܣ�win32����������¾��۹�����л�����Ұ����˱��
//                ���ߣ�ã����
//                ʱ�䣺2022/1/14
//                ���һ���޸�ʱ�䣺2022/09/30
/******************************************************************/

//-------------------------------��ͷ�ļ��������֡�-------------------------------
// �����������Ҫ��ͷ�ļ�
//-------------------------------------------------------------------------------
//windows������Ȼ��Ҫ����windowsͷ�ļ����Ա�ʹ��windowsAPI�������
#include <windows.h> 
#include <stdlib.h> 

//-------------------------------���궨�岿�֡�------------------------------------
// ����������һЩ�����ĺ�
//--------------------------------------------------------------------------------
//���������ʾ���ڵĴ�С
#define  WND_WIDTH   900
#define  WND_HEIGHT  700
//������
#define  RAINS_MAX_NUM   50

//�����ʾ����Ԫ�صĺ�

//-------------------------------��ȫ�ֽṹ���������֡�----------------------------
// ����������һЩ�ṹ��
//--------------------------------------------------------------------------------
typedef struct{
    int x;
    int y;
    int brightness;
}RAINS;

//-------------------------------��WndProc()����������----------------------------
// ��������������һ�����ڹ��̣��ص������������¼�����ʱ��Windows��ϵͳ���øú�����
//���ǿ���Ϊ���Ǹ���Ȥ���¼�д�ϴ�����롣
//--------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );





//����һ��WinMain������������c���Կ���̨����Ҫ����һ��main��������Ϊ��ں���
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    PSTR szCmdLine, int iCmdShow )
{
    /**************************************************************************/
    //Step1:����һ�������࣬��д���ڵ�����
    /**************************************************************************/
    HWND        hwnd;   //����һ�����ھ��
    MSG         msg;    //����һ����Ϣ�ṹ
    WNDCLASS   wndclass;    //����һ��������
    static TCHAR AppName[] = TEXT("codeRains"); //�����ǵĴ�����ȡһ������
    //����������ֶθ�ֵ
    wndclass.style              = CS_HREDRAW | CS_VREDRAW; //����Ϊ���ڴ�С�����ı�ʱҪ���»���
    wndclass.lpfnWndProc        = WndProc;  //Ϊ������󶨴��ڹ��̺���
    wndclass.cbClsExtra         = 0;    //��Ϊ0���ɣ������
    wndclass.cbWndExtra         = 0;    //��Ϊ0����
    wndclass.hInstance          = hInstance;
    //����������һ��Ĭ�ϵ�ͼ��
    
    //Ҳ����ΪӦ�ó����趨һ���Զ����ͼ��,����������Ҫ�ϸ��ico��ʽͼ���ļ���������ļ�����ͬһĿ¼��!!!
    wndclass.hIcon              = (HICON) LoadImage( NULL, TEXT("../picture/matrix.ico"), IMAGE_ICON, 
                                                    0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE );
    
    wndclass.hCursor            = LoadCursor( NULL, IDC_ARROW ); //ʹ��Ĭ�ϵ����ָ��
    //ѡ��һ��ϲ����ɫ�Ļ�ˢ��ϵͳ����������ɫ�����ǵĴ��ڱ���ˢ�ɺ�ɫ
    wndclass.hbrBackground      = (HBRUSH) GetStockObject( BLACK_BRUSH );    
    wndclass.lpszClassName      = AppName;  //ǰ���Ѿ�ȡ���˵�����
    /*****************************************************************************************/


    /*******************************************************************************/
    //Step2:ע�ᴰ���࣬����Windows���ǵĴ��������Ѿ��趨����
    /*******************************************************************************/
    if ( !RegisterClass( &wndclass ) )
    {
        // ���ע��ʧ�ܣ����������ʾ��Ϣ
        MessageBox( NULL, TEXT("error!!!"), TEXT("codeRains"), MB_ICONERROR );

        return 0;
    }

    //��������
    hwnd = CreateWindow ( AppName,               //����������
                          TEXT("codeRains"),     //���ڵı���
                          WS_OVERLAPPEDWINDOW,   //Ĭ�������ͺ���
                          CW_USEDEFAULT,         //���ڳ��ֵ�x����,��ΪĬ�ϵĺ���
                          CW_USEDEFAULT,         //���ڳ��ֵ�y����
                          WND_WIDTH,             //���ڵĿ��
                          WND_HEIGHT,            //���ڵĸ߶�
                          NULL,                  //ʣ������������
                          NULL,
                          hInstance,
                          NULL );

    ShowWindow( hwnd, iCmdShow );//��ʾ���ڣ����ʱ��ʵʵ���ڿ��Կ������ǵĴ�����
    //��������ʾ����
    UpdateWindow( hwnd );
    //��Ϣѭ��
    while ( GetMessage( &msg, NULL, 0 ,0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    } 

    return msg.wParam;
}



/*******************************************************************************/
//step3:�ڴ��ڹ�����д�������
/*******************************************************************************/

LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    HDC                 mdc,hdc;//�����豸dc�������ͼ
    PAINTSTRUCT              ps;
    static RECT            rect;
    HBITMAP                hBitmap;//����һ��λͼ���
    static RAINS     rains[RAINS_MAX_NUM] = { {0, 0, 0} };            
    int i;
    HPEN            pen;
    HBRUSH          hBrush;
    
    switch (message)
    {
        
        case WM_CREATE:
            GetClientRect( hwnd, &rect );
            //���������������λ�úͳ�ʼ����
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
                //���û�е��ﴰ�ڵײ����ͼ������䣬���ȼ�����ǿ
                if ( rains[i].y < 650 || rains[i].brightness < 255 )
                {
                    rains[i].y += 15;
                    rains[i].brightness += 20;
                }
                else//�����ʧ����������һ��
                { 
                    rains[i].x = rand() % 890;
                    rains[i].y = rand() % 600;
                    rains[i].brightness = rand() % 255;
                }
                
            }
            //��ӷ���WM_PAINT��Ϣ
            //����������ΪFALSE��ʾ�ػ�ʱ������ԭ����ʾ������
            InvalidateRect( hwnd, NULL, FALSE );
            break;
        case WM_PAINT:
            hdc = BeginPaint( hwnd, &ps );//��ͼ�Ӵ˴���ʼ
            pen = CreatePen( PS_INSIDEFRAME, 1, RGB( 255, 0, 0 ) );//������ɫ����
            SelectObject( hdc, pen );//������ѡ�뵱ǰ�豸
            hBrush = CreateSolidBrush( RGB( 0, 255, 0 ) );//������ɫ��ˢ
            SelectObject( hdc, hBrush);//����ˢѡ�뵱ǰ�豸
            //׼����ʾһ�ź���
            hBitmap = (HBITMAP)LoadImage( NULL,
                                          TEXT("../picture/matrix.bmp"),//�����ļ���
                                          IMAGE_BITMAP,//ͼƬ����
                                          420,//ͼƬ�ĳ���/����
                                          623,//ͼƬ�Ŀ��
                                          LR_LOADFROMFILE );//�ӱ����ļ�����
            mdc = CreateCompatibleDC( hdc );
            SelectObject ( mdc, hBitmap );
            BitBlt( hdc, 250, 0, 420, 623, mdc, 0, 0, SRCCOPY );//�Ѻ�����ͼ����Ļ������
            //��������
            for (i=0; i<RAINS_MAX_NUM; i++)
            {
                pen = CreatePen( PS_INSIDEFRAME, 1, RGB( 0, rains[i].brightness, 0 ) );
                SelectObject( hdc, pen );
                hBrush = CreateSolidBrush( RGB(0, rains[i].brightness, 0) );
                SelectObject( hdc, hBrush );
                //͵����������ɫ���δ����ַ�����Ҫ��������ҵ��ûʱ��Ľ�<*,*>����
                Rectangle( hdc, rains[i].x, rains[i].y, rains[i].x + 5, rains[i].y + 5 );
            }
            DeleteDC( mdc );
            DeleteObject( hBitmap );
            EndPaint( hwnd, &ps );//��ͼ����
            break;

     
        case WM_DESTROY://�ڴ˴������ƺ���
            KillTimer( hwnd, 1 );
            PostQuitMessage( 0 );//��֤���򳹵��˳�
            break;
        
    }
    return DefWindowProc( hwnd, message, wParam, lParam );
}




