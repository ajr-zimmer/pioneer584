/*
 * Code provided by Laura Berry & Christopher Miller
 * Modified by Sumeet Patel & Andrew Zimmer
*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using AForge.Imaging;
using AForge.Video;


namespace RobotTracker
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        AForge.Video.DirectShow.FilterInfoCollection videoDevices;
        AForge.Video.DirectShow.VideoCaptureDevice vidsource1, videosource2;

        public MainWindow()
        {
            InitializeComponent();



            // Videofeed Code

            //find video device
            videoDevices = new AForge.Video.DirectShow.FilterInfoCollection(AForge.Video.DirectShow.FilterCategory.VideoInputDevice);            
            // create video source
            vidsource1 = new AForge.Video.DirectShow.VideoCaptureDevice(videoDevices[0].MonikerString);
            videosource2 = new AForge.Video.DirectShow.VideoCaptureDevice(videoDevices[0].MonikerString);
            // enumerate video devices
            AForge.Video.DirectShow.FilterInfoCollection videoDevices2 = new AForge.Video.DirectShow.FilterInfoCollection(AForge.Video.DirectShow.FilterCategory.VideoInputDevice);
            // create video source
            AForge.Video.DirectShow.VideoCaptureDevice videoSource = new AForge.Video.DirectShow.VideoCaptureDevice(videoDevices2[0].MonikerString);
            // start the video source
            videoSource.Start();
            int counter = 0;
            while (counter < 2)
            {
                // set NewFrame event handler
                videoSource.NewFrame += new AForge.Video.NewFrameEventHandler(video_NewFrame);
                counter++;
            }

            // End of Videofeed code
        }

        private void video_NewFrame(object sender, NewFrameEventArgs eventArgs)
        {
            try
            {
                System.Drawing.Image image = (Bitmap)eventArgs.Frame.Clone();                   //Width==640 , Height==480
                System.IO.MemoryStream mssg = new System.IO.MemoryStream();
                image.Save(mssg, System.Drawing.Imaging.ImageFormat.Bmp);
                mssg.Seek(0, System.IO.SeekOrigin.Begin);
                BitmapImage bitmap1 = new BitmapImage();
                bitmap1.BeginInit();
                bitmap1.StreamSource = mssg;                                                    //creates a bitmap of the frame from the camera
                bitmap1.EndInit();

                bitmap1.Freeze();
                Dispatcher.BeginInvoke(new System.Threading.ThreadStart(delegate { VideoFrame.Source = bitmap1; })); //sets image source to be the bitmap image
            }
            catch (Exception ex)
            {
                Console.Write(ex);
            }
            System.Drawing.Bitmap bitmap = eventArgs.Frame;
            // create filter
            AForge.Imaging.Filters.ColorFiltering colorFilter = new AForge.Imaging.Filters.ColorFiltering();
            // configure the filter

            System.Drawing.Bitmap filteredImage = colorFilter.Apply(bitmap);

            /// create blob counter and configure it
            BlobCounter blobCounter = new BlobCounter();
            blobCounter.MinHeight = 5;
            blobCounter.MinWidth = 5;
            blobCounter.FilterBlobs = true;                                         // filter blobs by size
            blobCounter.ObjectsOrder = ObjectsOrder.Size;                           // order found object by size
            // grayscaling
            AForge.Imaging.Filters.Grayscale grayFilter = new AForge.Imaging.Filters.Grayscale(0.2125, 0.7154, 0.0721); ;

            Bitmap grayImage = grayFilter.Apply(filteredImage);
            // locate blobs 
            blobCounter.ProcessImage(grayImage);
            System.Drawing.Rectangle[] rects = blobCounter.GetObjectsRectangles();
            // draw rectangle around the biggest blob           //todo maybe alter this if it is not seeing the roomba properly


            if (rects.Length > 0)
            {
                for (int i = 0; i < rects.Length; i++)
                {
                    System.Drawing.Rectangle objectRect = rects[i];
                    Graphics g = Graphics.FromImage(bitmap);

                    using (System.Drawing.Pen pen = new System.Drawing.Pen(System.Drawing.Color.FromArgb(160, 255, 160), 3))
                    {
                        g.DrawRectangle(pen, objectRect);
                        int x1 = (objectRect.Left + objectRect.Right) / 2;                          //finds the x coordinate of the middle of the rectangle
                        int y1 = (objectRect.Top + objectRect.Bottom) / 2;                          //finds the y coordinate of the middle of the rectangle
                    }
                    g.Dispose();
                }
            }            
        }
    } 
}


