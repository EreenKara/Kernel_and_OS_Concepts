package Chapter3_Projects.Assignments.Programming_Exercises.Sockets;
/**
 * Client program requesting current date from server.
 *
 * Figure 3.28
 *
 * @author Silberschatz, Gagne and Galvin
 * Operating System Concepts  - Tenth Edition
 */ 

import java.net.*;
import java.io.*;

public class Quote_of_the_day_client
{
	public static void main(String[] args)  {
		try {
			// this could be changed to an IP name or address other than the localhost
            // baglanmak istedigin ip address'i ve port numarasi. 
            // 127.0.0.1 ozel bir ip address'idir. Bu ip adresi yine cihazin kendisini isaret eder. 
            // Yani loopback yapip yine cihaz'in kendisine gelecek paket.
			Socket sock = new Socket("127.0.0.1",6017);
            // in-stream pipe'larini uc uca doseyip okuma yapiyoruz.
			InputStream in = sock.getInputStream();
			BufferedReader bin = new BufferedReader(new InputStreamReader(in));

			String line;
			while( (line = bin.readLine()) != null)
				System.out.println(line);
				
			sock.close();
		}
		catch (IOException ioe) {
				System.err.println(ioe);
		}
	}
}
