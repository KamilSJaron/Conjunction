#ifndef CHIASMATA_H
#define CHIASMATA_H

class Chiasmata
{
	public :
/* INITIATION */
		Chiasmata(); // no recombination - empty vector
		Chiasmata(std::vector<int> input_chiasmata);
		~Chiasmata();

/* COMUNICATION METHODS */
		int length() const;
		int get(int index) const;
		void set(std::vector<int> input_chiasmata);
		void add(int chiasma);
		std::string collapse() const;

	protected:
		std::vector <int> chiasmata;
};

#endif
