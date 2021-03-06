#include <iostream>
#include <cassert>


struct MyString
{
	char *strings;
	short offset;
	short length;
};



void ad(FILE *fout);
short sizeof_file(FILE *fin);
void input(char *buff, short size, FILE *fin, FILE *fout);
void SortStr();
short count_str(char *buff, short size);
void Put_Pointer(char *buff, short count_of_str, short size, MyString* str);
int Compare_Function_Begin(const void* point_char1, const void* point_char2);
int Compare_Function_End(const void* point_char1, const void* point_char2);
void Print(MyString* str, short count_of_str, FILE *fout);
void PrintBegin(FILE *fout);
void PrintEnd(FILE *fout);
void PrintBefore(FILE *fout);



int main()
{
	SortStr();
}



//+++++++++++++++++++++++++++++++++++++//
//This function makes an advertisement //
//of this sort function                //
//+++++++++++++++++++++++++++++++++++++//


void ad(FILE *fout)
{
	fprintf(fout, "\t\t\t\tThis function can sort strings.\n"
		"\t\t\tPlease put the text into the file \"INPUT.txt\".\n");
}

//++++++++++++++++++++++++++++++++++++++++++//
//This function counts the digit of strings //
//in the input file                         //
//++++++++++++++++++++++++++++++++++++++++++//

short sizeof_file(FILE *fin)
{
	fseek(fin, 0, SEEK_END);

	short size(ftell(fin));

	fseek(fin, 0, SEEK_SET);

	assert(size > 0);

	return size;
}

//+++++++++++++++++++++++++++++//
//This function reads the text //
//from the input file          //
//+++++++++++++++++++++++++++++//


void input(char *buff, short size, FILE *fin, FILE *fout)
{
	fread(buff, sizeof(char), size, fin);
}



//+++++++++++++++++++++++++++++++++++//
//This function as a main can        //
//communicate with another functions //
//+++++++++++++++++++++++++++++++++++//

void SortStr()
{
	FILE *fin, *fout;
	fopen_s(&fin, "INPUT.txt", "rb");
	fopen_s(&fout, "OUTPUT.txt", "w");
	
	ad(fout);

	if ((fin == NULL)||(fout==NULL))
	{
		fprintf(fout, "Error! File was not open!");
	}
	else
	{
		short sizebuff = sizeof_file(fin) + 1;
		if (sizebuff)
		{
			char *buff = (char*)calloc(sizebuff, sizeof(char));

			if (!buff)
				fprintf(fout, "\nError! Malloc has not sing the memory\n");

			input(buff, sizebuff, fin, fout);

			short count_of_str = count_str(buff, sizebuff);//strchr

			assert(count_of_str);

			MyString* str = (MyString*)calloc(count_of_str, sizeof(*str));
			
			assert(str);

			Put_Pointer(buff, count_of_str, sizebuff, str);

			for (int counter(0); counter < count_of_str; ++counter)
				str[counter].strings = &buff[str[counter].length];

			PrintBefore(fout);

			Print(str, count_of_str, fout);

			PrintBegin(fout);

			qsort(str, count_of_str, sizeof(*str), Compare_Function_Begin);
			Print(str, count_of_str, fout);

			PrintEnd(fout);

			qsort(str, count_of_str, sizeof(*str), Compare_Function_End);
			Print(str, count_of_str, fout);

			free(buff);
			free(str);
		}
	}
	fclose(fin);
	fclose(fout);
}


//+++++++++++++++++++++++++++++++++++++++++//
//This function as count number of strings //
//and put '\0' instead '\n'                //
//+++++++++++++++++++++++++++++++++++++++++//


short count_str(char *buff, short size)
{
	short count_of_str(0);
	for (short counter(0); counter<size; counter++)
		if (buff[counter] == '\n')
		{
			count_of_str++;
			buff[counter] = '\0';
		}
	if (count_of_str != 1)
		count_of_str++;

	assert(count_of_str > 0);

	return count_of_str;
}



//+++++++++++++++++++++++++++++++++++++++++//
//This function put pointers on strings    //
//in array *strings                        //
//+++++++++++++++++++++++++++++++++++++++++//


void Put_Pointer(char *buff, short count_of_str, short size, MyString* str)
{
	assert(count_of_str > 0);
	short count_points(1);
	str[0].length = 0;
	short counter = 0;
	for (counter = 1; counter < size; ++counter)
	{
		if ((buff[counter] == '\0') && (count_points < count_of_str))// !!!!!!!
		{
			str[count_points].length = counter + 1;
			count_points++;
		}
	}
	for (counter = 1; counter < count_of_str; ++counter)
	{
		str[counter - 1].offset = str[counter].length - str[counter - 1].length-1;
	}
	str[count_of_str - 1].offset = size - str[count_of_str - 1].length;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++//
//Check the first letter on space, simbol or digit //
//and return strrcmp()                             //
//+++++++++++++++++++++++++++++++++++++++++++++++++//


int Compare_Function_Begin(const void* point_char1, const void* point_char2)
{
	MyString str1 = *(MyString*)point_char1;
	MyString str2 = *(MyString*)point_char2;
	while ((isdigit(*(str1).strings)) || (isspace(*(str1).strings))
		|| (ispunct(*(str1).strings)))
		(str1.strings)++;
	while ((isdigit(*(str2).strings)) || (isspace(*(str2).strings))
		|| (ispunct(*(str2).strings)))
		(str2.strings)++;
	return strcmp(str1.strings, str2.strings);
}


//++++++++++++++++++++++++++++++++++++++++++++//
//Check last letter on space, simbol or digit //
//and return difference from strings          //
//++++++++++++++++++++++++++++++++++++++++++++//



int Compare_Function_End(const void* point_char1, const void* point_char2)
{
	const short misstep(2);
	MyString str1 = *(MyString*)point_char1;
	MyString str2 = *(MyString*)point_char2;
	int counter1(misstep), counter2(misstep);
	while ((isspace(str1.strings[str1.offset - counter1])
		||(isdigit(str1.strings[str1.offset - counter1]))
			||(ispunct(str1.strings[str1.offset - counter1]))))counter1++;


	while ((isspace(str2.strings[str2.offset - counter2])
		|| (isdigit(str2.strings[str2.offset - counter2]))
		|| (ispunct(str2.strings[str2.offset - counter2]))))counter2++;
	
	
	while (str1.strings[str1.offset - counter1]
		== str2.strings[str2.offset - counter2])
	{
		counter1++;
		counter2++;
	}
	return (str1.strings[str1.offset - counter1] - str2.strings[str2.offset - counter2]);
}




//++++++++++++++//
//Print strings //
//++++++++++++++//


void Print(MyString* str, short count_of_str, FILE *fout)
{
	for (int counter(0); counter < count_of_str; counter++)
		fprintf(fout, "%s\n", str[counter].strings);
}

void PrintBefore(FILE *fout)
{
	fprintf(fout, "\n\n\nText before sort:\n\n\n\n");
}

void PrintBegin(FILE *fout)
{
	fprintf(fout, "\n\n\nText after ascending sort:\n\n\n\n");
}


void PrintEnd(FILE *fout)
{
	fprintf(fout, "\n\n\nText after sort from the end (rhymes dictionary):\n\n\n\n");
}
