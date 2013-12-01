


#include <string>
#include <cstdint>   
#include <stdlib.h> // ������ ���� rand()
#include <time.h> // ����� ��������� time()



#pragma once

using namespace std;



namespace Cards_processor{

	class Deck{
	  /*��������� ����: ����� ������������ ����� 1 ���� (uint8_t)
	  0,1,2,3 ���� �������� ����������� (2..����)
	  2=1; 3=2; 4=3; 5=4; 6=5; 7=6; 8=7; 9=8; T=9; J=10; Q=11; K=12; A=13;
	  4,5 ���� �������� �����
	  ?=0; ?=1; ?=2; ?=3;*/
	public:
		Deck();
		bool RemoveCard(uint8_t card);
		void Reload();
		uint8_t GetCard();
	private:
		uint8_t cards[52];
		void Shuffle();
	};

	Deck::Deck(){
		srand((unsigned)time(NULL)); //��� � ������� - ������� �������� ���� ������������ ������ �����
		Reload();
	}
	void Deck::Reload(void){
		//��������� ������ ������� ���������������
		for (int i=0; i<=12; i++){cards[i] = i+1;}; //����
		for (int i=13; i<=25; i++){cards[i] = (i-13+1)+16;};//�����
		for (int i=26; i<=38; i++){cards[i] = (i-26+1)+32;};//�����
		for (int i=39; i<=51; i++){cards[i] = (i-39+1)+16+32;};//�����
	
		Shuffle(); Shuffle(); Shuffle(); //���� ������ 3 ����!
	}
	uint8_t Deck::GetCard(){
		uint8_t buff=0;
		for (int i=0; i<=51; i++){
			if(cards[i]!=255){buff = cards[i]; cards[i]=255; return buff;}
		}
		return 0; //������, � ������ ��� ����
	}
	void Deck::Shuffle(){
		uint8_t buff, index;
		for(int i=0; i<=51; i++){
			index = rand()%52;
			buff = cards[i];
			cards[i]=cards[index];
			cards[index]=buff;
		}
	}
	bool Deck::RemoveCard(uint8_t card){
		for (int i=0; i<=51; i++){
			if(cards[i]==card){cards[i]=0; return 1;}
		}
		return 0;
	}

	//====================================================================

	uint8_t CardFromStr(string card);
	string Card_to_string(uint8_t card);
	uint32_t GetHand(uint8_t card1, uint8_t card2, uint8_t card3, uint8_t card4, uint8_t card5, uint8_t card6, uint8_t card7, string& opis);

	uint8_t IsStreet(uint32_t cards);
	uint8_t GetHiCard(uint32_t cards);
	uint8_t GetAndRemoveHiCard(uint32_t* cards);

	//==================================================================



	string Card_to_string(uint8_t card){
		uint8_t suit, value;
		string otvet;
		suit = (card>>4);
		value = (card&0xF);
		if (value == 1) otvet += '2';
		if (value == 2) otvet += '3';
		if (value == 3) otvet += '4';
		if (value == 4) otvet += '5';
		if (value == 5) otvet += '6';
		if (value == 6) otvet += '7';
		if (value == 7) otvet += '8';
		if (value == 8) otvet += '9';
		if (value == 9) otvet += 'T';
		if (value == 10) otvet += 'J';
		if (value == 11) otvet += 'Q';
		if (value == 12) otvet += 'K';
		if (value == 13) otvet += 'A';
		if (suit == 0) otvet += 's';//?
		if (suit == 1) otvet += 'c';//?
		if (suit == 2) otvet += 'd';//?
		if (suit == 3) otvet += 'h';//?
	return otvet;
	}



	uint8_t CardFromStr(string card){
		uint8_t buff = 0;
		if (card[0]=='2') buff=1;
		if (card[0]=='3') buff=2;
		if (card[0]=='4') buff=3;
		if (card[0]=='5') buff=4;
		if (card[0]=='6') buff=5;
		if (card[0]=='7') buff=6;
		if (card[0]=='8') buff=7;
		if (card[0]=='9') buff=8;
		if (card[0]=='T') buff=9;
		if (card[0]=='J') buff=10;
		if (card[0]=='Q') buff=11;
		if (card[0]=='K') buff=12;
		if (card[0]=='A') buff=13;
		if (buff==0) return 0;
		if (card[1]=='s') buff+=0;
		if (card[1]=='c') buff+=16;
		if (card[1]=='d') buff+=32;
		if (card[1]=='h') buff+=32+16;
		return buff;
	}


	
	
	uint32_t GetHand(uint8_t card1, uint8_t card2, uint8_t card3, uint8_t card4, uint8_t card5, uint8_t card6, uint8_t card7, string& opis){
	// ����������� 5 ���� � ���� ���� ���� (32 ����)
	  //������� 13 ��� ��������� �������� ���� ������������ �����������
		/*��������� ����: ����� ������������ ����� 1 ���� (char)
	  0,1,2,3 ���� �������� ����������� (2..����)
	  2=1; 3=2; 4=3; 5=4; 6=5; 7=6; 8=7; 9=8; T=9; J=10; Q=11; K=12; A=13;
	  4,5 ���� �������� �����
	  ?=0; ?=1; ?=2; ?=3;*/

	uint32_t otvet=0;
	uint32_t temp1=0;
	uint32_t temp2=0;

	uint32_t spades=0;
	uint32_t clubs=0;
	uint32_t diamonds=0;
	uint32_t hearts=0;

	if(card1>>4==0)spades|=(1<<((card1&0xF)-1));
	if(card1>>4==1)clubs|=(1<<((card1&0xF)-1));
	if(card1>>4==2)diamonds|=(1<<((card1&0xF)-1));
	if(card1>>4==3)hearts|=(1<<((card1&0xF)-1));

	if(card2>>4==0)spades|=(1<<((card2&0xF)-1));
	if(card2>>4==1)clubs|=(1<<((card2&0xF)-1));
	if(card2>>4==2)diamonds|=(1<<((card2&0xF)-1));
	if(card2>>4==3)hearts|=(1<<((card2&0xF)-1));

	if(card3>>4==0)spades|=(1<<((card3&0xF)-1));
	if(card3>>4==1)clubs|=(1<<((card3&0xF)-1));
	if(card3>>4==2)diamonds|=(1<<((card3&0xF)-1));
	if(card3>>4==3)hearts|=(1<<((card3&0xF)-1));

	if(card4>>4==0)spades|=(1<<((card4&0xF)-1));
	if(card4>>4==1)clubs|=(1<<((card4&0xF)-1));
	if(card4>>4==2)diamonds|=(1<<((card4&0xF)-1));
	if(card4>>4==3)hearts|=(1<<((card4&0xF)-1));

	if(card5>>4==0)spades|=(1<<((card5&0xF)-1));
	if(card5>>4==1)clubs|=(1<<((card5&0xF)-1));
	if(card5>>4==2)diamonds|=(1<<((card5&0xF)-1));
	if(card5>>4==3)hearts|=(1<<((card5&0xF)-1));

	if(card6>>4==0)spades|=(1<<((card6&0xF)-1));
	if(card6>>4==1)clubs|=(1<<((card6&0xF)-1));
	if(card6>>4==2)diamonds|=(1<<((card6&0xF)-1));
	if(card6>>4==3)hearts|=(1<<((card6&0xF)-1));

	if(card7>>4==0)spades|=(1<<((card7&0xF)-1));
	if(card7>>4==1)clubs|=(1<<((card7&0xF)-1));
	if(card7>>4==2)diamonds|=(1<<((card7&0xF)-1));
	if(card7>>4==3)hearts|=(1<<((card7&0xF)-1));

	//printf("spades=%d \n",spades);
	//��������� ���� �� �����-����

	if (temp1=IsStreet(spades)){opis = "straight-flush"; otvet=(8<<28)|temp1; return otvet;};
	if (temp1=IsStreet(clubs)){opis = "straight-flush"; otvet=(8<<28)|temp1; return otvet;};
	if (temp1=IsStreet(diamonds)){opis = "straight-flush"; otvet=(8<<28)|temp1; return otvet;};
	if (temp1=IsStreet(hearts)){opis = "straight-flush"; otvet=(8<<28)|temp1; return otvet;};

	//��������� ���� �� 4(����)
	if(temp1=spades&clubs&diamonds&hearts){
	  temp2 = spades|clubs|diamonds|hearts;
	  temp2&=~temp1;//����� ���� 4��
	  uint8_t t;
	  t = GetHiCard(temp2);
	  otvet = (7<<28)|(temp1<<13)|t;
	  opis = "4";
	  return otvet;
	}

	uint32_t troiki=0;
	troiki = (spades&clubs&diamonds)|(spades&clubs&hearts)|(spades&diamonds&hearts)|(clubs&diamonds&hearts);
	uint32_t dvoiki=0;
	dvoiki = (spades&clubs)|(spades&diamonds)|(spades&hearts)|(clubs&diamonds)|(clubs&hearts)|(diamonds&hearts);

	//��������� ���� �� 3+2
	if (troiki){
	  //������� ������� ������
	  uint8_t t;
	  t = GetHiCard(troiki);
	  //���� ������
	  dvoiki&=~(1<<t);//����� � ������� ������� ������ (��������� ������ ��������)
	  if (dvoiki){
		otvet = (6<<28)|(t<<20)|dvoiki;
		opis = "3+2";
		return otvet;
	  }
	}
	//��������� ���� �� flash
	uint8_t s=0,c=0,d=0,h=0;
	for(int i=0;i<=12;i++){
	  if(spades&(1<<i))s++;
	  if(clubs&(1<<i))c++;
	  if(diamonds&(1<<i))d++;
	  if(hearts&(1<<i))h++;
	}
	//printf("spades=%d s=%d d=%d h=%d\n",spades,s,d,h);
	if(s>=5){
		otvet = (5<<28);
		uint8_t t;
		t = GetAndRemoveHiCard(&spades);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&spades);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&spades);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&spades);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&spades);
		otvet|=(1<<t);
		opis = "flush";
		return otvet;
	}
	if(c>=5){
		otvet = (5<<28);
		uint8_t t;
		t = GetAndRemoveHiCard(&clubs);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&clubs);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&clubs);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&clubs);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&clubs);
		otvet|=(1<<t);
		opis = "flush";
		return otvet;
	}
	if(d>=5){
		otvet = (5<<28);
		uint8_t t;
		t = GetAndRemoveHiCard(&diamonds);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&diamonds);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&diamonds);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&diamonds);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&diamonds);
		otvet|=(1<<t);
		opis = "flush";
		return otvet;
	}
	if(h>=5){
		otvet = (5<<28);
		uint8_t t;
		t = GetAndRemoveHiCard(&hearts);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&hearts);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&hearts);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&hearts);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&hearts);
		otvet|=(1<<t);
		opis = "flush";
		return otvet;
	}

	//��������� ���� �� �����
	if(temp1 = IsStreet(spades|clubs|diamonds|hearts)){otvet=(4<<28)|temp1; opis = "straight"; return otvet;}

	//��������� ���� �� 3
	if (troiki){//���� �� ����� ����, �� � ��� ����� ���� ������ 1 ������
	  otvet = (3<<28)|(troiki<<13);
	  uint32_t r = spades|clubs|diamonds|hearts;
	  r&=~troiki;//����� ������
	  uint8_t t;
	  t = GetAndRemoveHiCard(&r);
	  otvet|=(1<<t);
	  t = GetAndRemoveHiCard(&r);
	  otvet|=(1<<t);
	  t = GetAndRemoveHiCard(&r);
	  otvet|=(1<<t);
	  opis = "3";
	  return otvet;
	}


	//��������� ���� �� 2+2 ��� 2
	if (dvoiki){
  
	  uint32_t r = spades|clubs|diamonds|hearts;
	  uint8_t ost;
	  //������� ������� ������
	  uint8_t t;
	  t = GetAndRemoveHiCard(&dvoiki);
	  if (dvoiki){//���� ���� ������ ������ ����� ���������
		uint8_t t2;
		t2 = GetAndRemoveHiCard(&dvoiki);
		r&=~(1<<t);//����� ������� ������
		r&=~(1<<t2);//����� ������� ������
		ost = GetAndRemoveHiCard(&r);
		otvet = (2<<28)|(t<<24)|(t2<<20)|ost;
		opis = "2+2";
		return otvet;
	  }
	  otvet = (1<<28)|(t<<24);
	  r&=~(1<<t);//����� ������
	  ost = GetAndRemoveHiCard(&r);
	  otvet|=(1<<ost);
	  ost = GetAndRemoveHiCard(&r);
	  otvet|=(1<<ost);
	  ost = GetAndRemoveHiCard(&r);
	  otvet|=(1<<ost);
	  opis = "2";
	  return otvet;
	}

	//���� ������ ��� (������� �����)
	uint32_t r = spades|clubs|diamonds|hearts;
	uint8_t ost;
	otvet = (0<<28);
	ost = GetAndRemoveHiCard(&r);
	otvet|=(1<<ost);
	ost = GetAndRemoveHiCard(&r);
	otvet|=(1<<ost);
	ost = GetAndRemoveHiCard(&r);
	otvet|=(1<<ost);
	ost = GetAndRemoveHiCard(&r);
	otvet|=(1<<ost);
	ost = GetAndRemoveHiCard(&r);
	otvet|=(1<<ost);
	opis = "Hi card";
	return otvet;
	}
	uint32_t GetHand(uint8_t card1, uint8_t card2, uint8_t card3, uint8_t card4, uint8_t card5, uint8_t card6, uint8_t card7){
	// ����������� 5 ���� � ���� ���� ���� (32 ����)
	  //������� 13 ��� ��������� �������� ���� ������������ �����������
		/*��������� ����: ����� ������������ ����� 1 ���� (char)
	  0,1,2,3 ���� �������� ����������� (2..����)
	  2=1; 3=2; 4=3; 5=4; 6=5; 7=6; 8=7; 9=8; T=9; J=10; Q=11; K=12; A=13;
	  4,5 ���� �������� �����
	  ?=0; ?=1; ?=2; ?=3;*/

	uint32_t otvet=0;
	uint32_t temp1=0;
	uint32_t temp2=0;

	uint32_t spades=0;
	uint32_t clubs=0;
	uint32_t diamonds=0;
	uint32_t hearts=0;

	if(card1>>4==0)spades|=(1<<((card1&0xF)-1));
	if(card1>>4==1)clubs|=(1<<((card1&0xF)-1));
	if(card1>>4==2)diamonds|=(1<<((card1&0xF)-1));
	if(card1>>4==3)hearts|=(1<<((card1&0xF)-1));

	if(card2>>4==0)spades|=(1<<((card2&0xF)-1));
	if(card2>>4==1)clubs|=(1<<((card2&0xF)-1));
	if(card2>>4==2)diamonds|=(1<<((card2&0xF)-1));
	if(card2>>4==3)hearts|=(1<<((card2&0xF)-1));

	if(card3>>4==0)spades|=(1<<((card3&0xF)-1));
	if(card3>>4==1)clubs|=(1<<((card3&0xF)-1));
	if(card3>>4==2)diamonds|=(1<<((card3&0xF)-1));
	if(card3>>4==3)hearts|=(1<<((card3&0xF)-1));

	if(card4>>4==0)spades|=(1<<((card4&0xF)-1));
	if(card4>>4==1)clubs|=(1<<((card4&0xF)-1));
	if(card4>>4==2)diamonds|=(1<<((card4&0xF)-1));
	if(card4>>4==3)hearts|=(1<<((card4&0xF)-1));

	if(card5>>4==0)spades|=(1<<((card5&0xF)-1));
	if(card5>>4==1)clubs|=(1<<((card5&0xF)-1));
	if(card5>>4==2)diamonds|=(1<<((card5&0xF)-1));
	if(card5>>4==3)hearts|=(1<<((card5&0xF)-1));

	if(card6>>4==0)spades|=(1<<((card6&0xF)-1));
	if(card6>>4==1)clubs|=(1<<((card6&0xF)-1));
	if(card6>>4==2)diamonds|=(1<<((card6&0xF)-1));
	if(card6>>4==3)hearts|=(1<<((card6&0xF)-1));

	if(card7>>4==0)spades|=(1<<((card7&0xF)-1));
	if(card7>>4==1)clubs|=(1<<((card7&0xF)-1));
	if(card7>>4==2)diamonds|=(1<<((card7&0xF)-1));
	if(card7>>4==3)hearts|=(1<<((card7&0xF)-1));

	//printf("spades=%d \n",spades);
	//��������� ���� �� �����-����

	if (temp1=IsStreet(spades)){otvet=(8<<28)|temp1; return otvet;};
	if (temp1=IsStreet(clubs)){otvet=(8<<28)|temp1; return otvet;};
	if (temp1=IsStreet(diamonds)){otvet=(8<<28)|temp1; return otvet;};
	if (temp1=IsStreet(hearts)){otvet=(8<<28)|temp1; return otvet;};

	//��������� ���� �� 4(����)
	if(temp1=spades&clubs&diamonds&hearts){
	  temp2 = spades|clubs|diamonds|hearts;
	  temp2&=~temp1;//����� ���� 4��
	  uint8_t t;
	  t = GetHiCard(temp2);
	  otvet = (7<<28)|(temp1<<13)|t;
	  return otvet;
	}

	uint32_t troiki=0;
	troiki = (spades&clubs&diamonds)|(spades&clubs&hearts)|(spades&diamonds&hearts)|(clubs&diamonds&hearts);
	uint32_t dvoiki=0;
	dvoiki = (spades&clubs)|(spades&diamonds)|(spades&hearts)|(clubs&diamonds)|(clubs&hearts)|(diamonds&hearts);

	//��������� ���� �� 3+2
	if (troiki){
	  //������� ������� ������
	  uint8_t t;
	  t = GetHiCard(troiki);
	  //���� ������
	  dvoiki&=~(1<<t);//����� � ������� ������� ������ (��������� ������ ��������)
	  if (dvoiki){
		otvet = (6<<28)|(t<<20)|dvoiki;
		return otvet;
	  }
	}
	//��������� ���� �� flash
	uint8_t s=0,c=0,d=0,h=0;
	for(int i=0;i<=12;i++){
	  if(spades&(1<<i))s++;
	  if(clubs&(1<<i))c++;
	  if(diamonds&(1<<i))d++;
	  if(hearts&(1<<i))h++;
	}
	//printf("spades=%d s=%d d=%d h=%d\n",spades,s,d,h);
	if(s>=5){
		otvet = (5<<28);
		uint8_t t;
		t = GetAndRemoveHiCard(&spades);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&spades);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&spades);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&spades);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&spades);
		otvet|=(1<<t);
		return otvet;
	}
	if(c>=5){
		otvet = (5<<28);
		uint8_t t;
		t = GetAndRemoveHiCard(&clubs);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&clubs);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&clubs);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&clubs);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&clubs);
		otvet|=(1<<t);
		return otvet;
	}
	if(d>=5){
		otvet = (5<<28);
		uint8_t t;
		t = GetAndRemoveHiCard(&diamonds);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&diamonds);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&diamonds);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&diamonds);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&diamonds);
		otvet|=(1<<t);
		return otvet;
	}
	if(h>=5){
		otvet = (5<<28);
		uint8_t t;
		t = GetAndRemoveHiCard(&hearts);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&hearts);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&hearts);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&hearts);
		otvet|=(1<<t);
		t = GetAndRemoveHiCard(&hearts);
		otvet|=(1<<t);
		return otvet;
	}

	//��������� ���� �� �����
	if(temp1 = IsStreet(spades|clubs|diamonds|hearts)){otvet=(4<<28)|temp1; return otvet;}

	//��������� ���� �� 3
	if (troiki){//���� �� ����� ����, �� � ��� ����� ���� ������ 1 ������
	  otvet = (3<<28)|(troiki<<13);
	  uint32_t r = spades|clubs|diamonds|hearts;
	  r&=~troiki;//����� ������
	  uint8_t t;
	  t = GetAndRemoveHiCard(&r);
	  otvet|=(1<<t);
	  t = GetAndRemoveHiCard(&r);
	  otvet|=(1<<t);
	  t = GetAndRemoveHiCard(&r);
	  otvet|=(1<<t);
	  return otvet;
	}


	//��������� ���� �� 2+2 ��� 2
	if (dvoiki){
  
	  uint32_t r = spades|clubs|diamonds|hearts;
	  uint8_t ost;
	  //������� ������� ������
	  uint8_t t;
	  t = GetAndRemoveHiCard(&dvoiki);
	  if (dvoiki){//���� ���� ������ ������ ����� ���������
		uint8_t t2;
		t2 = GetAndRemoveHiCard(&dvoiki);
		r&=~(1<<t);//����� ������� ������
		r&=~(1<<t2);//����� ������� ������
		ost = GetAndRemoveHiCard(&r);
		otvet = (2<<28)|(t<<24)|(t2<<20)|ost;
		return otvet;
	  }
	  otvet = (1<<28)|(t<<24);
	  r&=~(1<<t);//����� ������
	  ost = GetAndRemoveHiCard(&r);
	  otvet|=(1<<ost);
	  ost = GetAndRemoveHiCard(&r);
	  otvet|=(1<<ost);
	  ost = GetAndRemoveHiCard(&r);
	  otvet|=(1<<ost);
	  return otvet;
	}

	//���� ������ ��� (������� �����)
	uint32_t r = spades|clubs|diamonds|hearts;
	uint8_t ost;
	otvet = (0<<28);
	ost = GetAndRemoveHiCard(&r);
	otvet|=(1<<ost);
	ost = GetAndRemoveHiCard(&r);
	otvet|=(1<<ost);
	ost = GetAndRemoveHiCard(&r);
	otvet|=(1<<ost);
	ost = GetAndRemoveHiCard(&r);
	otvet|=(1<<ost);
	ost = GetAndRemoveHiCard(&r);
	otvet|=(1<<ost);
	return otvet;
	}

	uint8_t IsStreet(uint32_t cards){

	cards&=0x1FFF;
	if ((cards&0x1F00)==0x1F00)return 13;
	if ((cards&0xF80)==0xF80)return 12;
	if ((cards&0x7C0)==0x7C0)return 11;
	if ((cards&0x3E0)==0x3E0)return 10;
	if ((cards&0x1F0)==0x1F0)return 9;
	if ((cards&0xF8)==0xF8)return 8;
	if ((cards&0x7C)==0x7C)return 7;
	if ((cards&0x3E)==0x3E)return 6;
	if ((cards&0x1F)==0x1F)return 5;
	if ((cards&0x100F)==0x100F)return 4;
	return 0;
	}

	uint8_t GetHiCard(uint32_t cards){
	  uint8_t t=12;
	  while((cards&(1<<t))==0){t--;}//�� ������ t = ������� ����� (�� 12 �� 0)
	  return t;
	}
	uint8_t GetAndRemoveHiCard(uint32_t* cards){
	  uint8_t t=12;
	  while((*cards&(1<<t))==0){t--;}//�� ������ t = ������� ����� (�� 12 �� 0)
	  *cards&=~(1<<t);//����� ��� ������� ����� � cards
	  return t;
	}

}








