    /**
 * Time-of-day server listening to port 6013.
 *
 * Figure 3.27
 *
 * @author Silberschatz, Gagne, and Galvin. 
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018.
 */


package Chapter3_Projects.Assignments.Programming_Exercises.Sockets;
import java.net.*;
import java.util.Date;
import java.io.*;

public class Quote_of_the_day_server {
    public static void main(String[] args)  {
        try {
            // arbitrary olarak belirledigimiz bir port 6013. Istersen 1024'ten buyuk istedigini verebilrisin: 
            ServerSocket sock = new ServerSocket(6017);

            // now listen for connections
            while (true) {
                // accept() fonksiyonu kendisine bir istek gelene kadar bekler. 
                Socket client = sock.accept();
                // we have a connection
                
                // Bir stream olusturuyoruz. Ondan okuma ve yazma yapacagiz.
                PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
                // write the Date to the socket
                Date date = new Date();
                int gun = date.getDay();
                String message ="";
                switch (gun) {
                    case 0:
                    message = "Pazartesi";
                        break;
                    case 1:
                    message = "Sali";
                        break;
                    case 2:
                    message = "Carsamba";
                        break;
                    case 3:
                    message = "Persembe";
                        break;
                    case 4:
                    message = "Cuma";
                        break;
                    case 5:
                    message = "Cumartesi";
                        break;
                    case 6:
                    message = "Pazar";
                        break;
                    default:
                        break;
                }
                pout.println(message);
                // close the socket and resume listening for more connections
                client.close();
            }
        }
        catch (IOException ioe) {
                System.err.println(ioe);
        }
    }
}

