/**
 * Time-of-day server listening to port 6013.
 *
 * Figure 3.27
 *
 * @author Silberschatz, Gagne, and Galvin. 
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018.
 */
 
import java.net.*;
import java.io.*;

public class Socket_server
{
	public static void main(String[] args)  {
		try {
            // arbitrary olarak belirledigimiz bir port 6013. Istersen 1024'ten buyuk istedigini verebilrisin: 
			ServerSocket sock = new ServerSocket(6013);

			// now listen for connections
			while (true) {
                // accept() fonksiyonu kendisine bir istek gelene kadar bekler. 
				Socket client = sock.accept();
				// we have a connection
				
                // Bir stream olusturuyoruz. Ondan okuma ve yazma yapacagiz.
				PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
				// write the Date to the socket
				pout.println(new java.util.Date().toString());

				// close the socket and resume listening for more connections
				client.close();
			}
		}
		catch (IOException ioe) {
				System.err.println(ioe);
		}
	}
}
