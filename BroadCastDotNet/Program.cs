using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace BroadCastDotNet
{
    class Program
    {
        static void Main(string[] args)
        {
            Socket s = new Socket(SocketType.Dgram, ProtocolType.Udp);
            s.EnableBroadcast = true;
            bool connected = false;

            while(!connected)
            {
                string message = "anyonethere";
                s.SendTo(ASCIIEncoding.ASCII.GetBytes(message), new System.Net.IPEndPoint(IPAddress.Broadcast, 4000));

                byte[] buf = new byte[1024];
                EndPoint from = null;
                SocketAsyncEventArgs asyncEvent = new SocketAsyncEventArgs();
                asyncEvent.Completed += (o,e) =>
                {
                    Console.WriteLine("{0}: {1}",System.AppDomain.CurrentDomain.FriendlyName, ASCIIEncoding.ASCII.GetString(e.Buffer));
                };
                asyncEvent.SetBuffer(buf, 0, buf.Length);
                asyncEvent.RemoteEndPoint = new IPEndPoint(IPAddress.Any, 0);
                if (s.ReceiveFromAsync(asyncEvent))
                {
                    int a = 14;
                }
                Thread.Sleep(5000);
            }
        }
    }
}
