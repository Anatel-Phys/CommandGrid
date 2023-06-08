#include "Controller.h"


char com_port[] = "\\\\.\\COM5";


int main()
{
	Controller ctr(2, 2, com_port, com_port, CBR_115200);

	while (true)
	{
		ctr.run();
	}


	
}

//vizualisation for 2x2 grid and physical command grid
//int main()
//{
//	GridReader board(2, 2, com_port, CBR_115200);
//	sf::RenderWindow window(sf::VideoMode(1000, 1000), "CommandGrid");
//	sf::VertexArray arr(sf::Lines, 8);
//	sf::Event ev;
//
//	for (size_t i = 0; i < 8; i++)
//		arr[i].color = sf::Color::Green;
//	arr[0].position = { 50, 350 };
//	arr[1].position = { 950, 350 };
//	arr[2].position = { 50, 650 };
//	arr[3].position = { 950, 650 };
//	arr[4].position = { 350, 50 };
//	arr[5].position = { 350, 950 };
//	arr[6].position = { 650, 50 };
//	arr[7].position = { 650, 950 };
//
//
//
//	if (!board.connected_)
//	{
//		printf("No connection on port %s, aborting..", com_port);
//		return -1;
//	}
//	
//	std::cout << "Awake\n";
//
//	board.calibrate(30);
//
//
//	while (window.isOpen())
//	{
//		while (window.pollEvent(ev))
//		{
//			switch (ev.type)
//			{
//			default:
//				break;
//			case sf::Event::Closed:
//				window.close();
//				break;
//			}
//		}
//
//		PurgeComm(board.io_handler_, PURGE_RXCLEAR | PURGE_TXCLEAR);
//		Sleep(20);
//		if (!board.ReadSensorData(1))
//			std::cout << "Couldn't read sensor data\n";
//		board.fill_sensor_vals();
//
//
//		for (int i = 0; i < 4; i++)
//		{
//			if (board.obstructed(i))
//			{
//				arr[2*i].color = sf::Color::Red;
//				arr[2*i + 1].color = sf::Color::Red;
//			}
//			else
//			{
//				arr[2 * i].color = sf::Color::Green;
//				arr[2 * i + 1].color = sf::Color::Green;
//			}
//		}
//
//		window.clear();
//		window.draw(arr);
//		window.display();
//	}
//
//	return 0;
//}

//int main()
//{	
//	
//	Arduino board(com_port);
//
//	if (!board.simpleSerial->connected_)
//	{
//		std::cout << "No connexion on " << com_port << std::endl;
//		return -1;
//	}
//
//	std::vector<int> diodes = { 10,9,8,7,6,5,4,3,2 };
//	int n_diodes = diodes.size();
//	int alim = 11;
//	int comp = 0;
//	size_t n_mean = 2;
//	int millis_per_frame = 100;
//	int buf, min_V, max_V;
//
//	for (int d : diodes) {
//		board.pinMode(d, OUTPUT);
//		board.digitalWrite(d, LOW);
//	}
//	board.pinMode(alim, OUTPUT);
//	board.digitalWrite(alim, HIGH);
//
//	//std::cout << "SETUP : OBTRUATE THE LDR AND PRESS ENTER...";
//	//std::cin.get();
//	//std::cout << std::endl;
//	//buf = 0;
//	//for (size_t i = 0; i < n_mean + 1; i++) //plus one bcs for some reason first read is 0 everytime, TO FIX
//	//{
//	//	Sleep(10);
//	//	buf += board.analogRead(comp);
//	//}
//	//min_V = buf / n_mean;
//
//	//std::cout << "DONE. DE-OBTRUATE THE LDR AND PRESS ENTER...";
//	//std::cin.get();
//	//std::cout << std::endl;
//	//buf = 0;
//	//for (size_t i = 0; i < n_mean; i++)
//	//{
//	//	Sleep(10);
//	//	buf += board.analogRead(comp);
//	//}
//	//max_V = buf / n_mean;
//
//	max_V = 1000;
//	min_V = 0;
//
//	float interval = static_cast<float>(max_V - min_V) / n_diodes;
//
//	int cur = 0;
//	while (true)
//	{
//		buf = board.analogRead(comp);
//		std::cout << buf << std::endl;
//
//		//buf = min_V + cur * interval + 1;
//
//		for (int i = 0; i < n_diodes; i++)
//		{
//			if (buf > min_V + i * interval)
//			{
//				board.digitalWrite(diodes.at(i), HIGH);
//			}
//			else
//			{
//				board.digitalWrite(diodes.at(i), LOW);
//			}
//		}
//
//		cur++;
//		if (cur > n_diodes)
//			cur = 0;
//		Sleep(100);
//	}
//
//	return 0;
//}