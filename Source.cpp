// The text encryption program in C++ and ASM with a very simple example encryption method -
#include <string>					   // for strings
#include <fstream>                     // file I/O
#include <iostream>                    // for cin >> and cout <<
#include <iomanip>                     // for fancy output
//#include "TimeUtils.h"                 // for GetTime, GetDate, etc.
using namespace std;

char EKey = 'W';

// My Encryption Key is "W"

#define StudentName "Khalifa Almehairi"

//  *** PLEASE CHANGE THE NAME IN QUOTES ABOVE TO YOUR NAME, I have Changed my name   ***

#define MAXCHARS 20                    // the size can be anything of choice 

#define dollarchar '$'                 // string terminator

char OChars[MAXCHARS],                // Original character string
EChars[MAXCHARS],                     // Encrypted character string
DChars[MAXCHARS];                  // Decrypted character string

//----------------------------- C++ Functions ----------------------------------------------------------
void get_char(char& );
void get_original_chars(int&);
void encrypt_chars(int , char);
void decrypt_chars(int , char);



//************ MAIN PROGRAM *************************************************************************************

int main(void)
{
	int char_count(MAXCHARS);                  // The number of actual characters entered (upto MAXCHARS limit).

	cout << "\nPlease enter upto " << MAXCHARS << " alphanumeric characters:  ";

	get_original_chars(char_count);	// Input the original character string to be encrypted 

	//*****************************************************
	// Open a file to store results (you can view and edit this file in Visual Studio)

	std::ofstream EDump;
	EDump.open("EncryptDump.txt", std::ios::app);
	EDump << "\n\nFoMCA Encryption program results (" << StudentName << ") Encryption key = '" << EKey << "'";
	//EDump << "\nDate: " << GetDate() << "  Time: " << GetTime();

	//*****************************************************
	// Display and save to the EDump file the string just input

	cout << "\n\nOriginal string =  " << OChars << "\tHex = ";
	EDump << "\n\nOriginal string =  " << OChars << "\tHex = ";

	for (int i = 0; i < char_count; ++i)
	{
		cout << hex << setw(2) << setfill('0') << ((int(OChars[i])) & 0xFF) << "  ";
		EDump << hex << setw(2) << setfill('0') << ((int(OChars[i])) & 0xFF) << "  ";
	}

	//*****************************************************
	// Encrypt the string and display/save the result

	encrypt_chars(char_count, EKey);

	cout << "\n\nEncrypted string = " << EChars << "\tHex = ";
	EDump << "\n\nEncrypted string = " << EChars << "\tHex = ";
	for (int i = 0; i < char_count; ++i)
	{
		cout << ((int(EChars[i])) & 0xFF) << "  ";
		EDump << ((int(EChars[i])) & 0xFF) << "  ";
	}

	//*****************************************************
	// Decrypt the encrypted string and display/save the result
	decrypt_chars(char_count, EKey);	//**** YOU NEED TO WRITE THE BODY OF THIS FUNCTION ***
	cout << "\n\nDecrypted string = " << DChars << "\tHex = ";
	EDump << "\n\nDecrypted string = " << DChars << "\tHex = ";
	for (int i = 0; i < char_count; ++i)
	{
		cout << ((int(DChars[i])) & 0xFF) << "  ";
		EDump << ((int(DChars[i])) & 0xFF) << "  ";
	}
	//*****************************************************

	cout << "\n\n\n";
	EDump << "\n\n-------------------------------------------------------------";
	EDump.close();

	system("PAUSE");                  // ! just a hack to pause the program before exiting

	return 0;

} // end of whole encryption/decryption program --------------------------------------------------------------------


void get_char(char& a_character)
{
	a_character = (char)_getwche();
	if (a_character == '\r' || a_character == '\n')  // allow the enter key to work as the terminating character too
		a_character = dollarchar;
}

void get_original_chars(int& length)
{
	char next_char = ' ';
	length = 0;
	get_char(next_char);
	while ((length < MAXCHARS) && (next_char != dollarchar))
	{
		OChars[length++] = next_char;
		get_char(next_char);
	}
}
//---------------------------------------------------------------------------------------------------------------
//----------------- ENCRYPTION ROUTINES -------------------------------------------------------------------------
void encrypt_chars(int length, char EKey)
{
	char temp_char;                      // Character temporary store

	char* p;
	p = &EKey;
	for (int i = 0; i < length; ++i)     // Encrypt characters one at a time
	{

		temp_char = OChars[i];			   // its h=104 in first iteration
		if (temp_char >= 'A' && temp_char <= 'Z')
			temp_char = temp_char + 32;        // changing the upper case letters to lower case letters

		__asm
		{                                  // when we enter in a subroutine we push all the register in stack 
			push   eax                       // this frees the register while keeping the previous information reserved 
			push   ecx                       //we push the register and keep the order in mind
			push   edx

			movzx  ecx, temp_char            //the value "h"in first iteration will be stored in ecx and in the same way after every iteration this will be updated to the next character
			lea    eax, EKey                 //the address of the key=W is taken to the Eax the address is always changed in every iteration

			call   encrypt_4                 // we call the encryption routine 
			mov    temp_char, dl             // for 1st iteration 23 is loaded into temp char and 23 in hex is 17

											 // for 2nd temp char has value   204 or CC in hex

											 // for 3rd iteration 7 is loaded into temp char

											 // for 4thiteration  240 is loaded into temp char or CC in hex

											 // for 5th iteration 19 is loaded into temp char or 13 in hex
											 // we pushed the register while entering in this function now 
			pop    edx                       // we are poping them out in the same reverse order
			pop    ecx                       //doing so is important becasue it saves the important information 
			pop    eax                       //which was previusly present in the registers
		}

		EChars[i] = temp_char;                 // Store encrypted char in the Encrypted Chars array
	}
	return;
	__asm
	{
		encrypt_4:
		push  edi                           // the base pointer of stack is pushed into stack to keep it intact and use while exitting out of the function
											//this will be popped back when we are done in this routine

		push  ecx						     // the first character "CHAR" was stored in ecx               

		not byte ptr[eax]					//EAX is holding the EKey that is "W"
		add   byte ptr[eax], 0x04
		movzx edi, byte ptr[eax]			// this value is zero extended and stored in edi

		pop   edx
											// the first character "CHAR" was stored in ecx and now it is pop into edx
											// first interation it is holding h and in hex it is 0x68
		xor edx, edi						// the xor of 0x68 and 0xac is C4 and it is 196 in decimal

											// edx is holding 196 value
		pop   edi							// the edi which was previously pushed into stack is now again retrived back
		rol   dl, 1							//196 is rol by 1 and the output is 137
		rol   dl, 1						    // 137 is rol and output is 19
		add   dl, 0x04						//19+4=23
		ret
	}

}
//---------------------------------------end of encrypt_chars function------------------------------------------------------------------------

//----------------- DECRYPTION ROUTINES -------------------------------------------------------------------------
void decrypt_chars(int length, char EKey)
{
	char temp_char;						  // Character temporary store
	char* p;
	p = &EKey;
	for (int i = 0; i < length; ++i)     // Now we will do the exact opposite of encryption in Decryption
	{

		temp_char = EChars[i];         //the Echars is holding the string of encrypted string

		__asm
		{                              //Here we are again pushing the encrypted string into the stack
			push   eax                 //note the order in which stack is being filled
			push   ecx
			push   edx

			movzx  ecx, temp_char      //the value "h"in first iteration will be stored in ecx and in the same way after every iteration this will be updated to the next character
			push ecx                   //the address of the key=W is taken to the Eax the address is always changed in every iteration

									   //ecx is holding the char and it is pushed into stack
			lea    eax, EKey
			push eax
										// we have pushed the parameter into stack and now we will not use eax directly into code
			call   dencrypt_4          //We are calling the decryption label so we may do the decrytpion there

			add esp, 8                 //now we will free the space occupied by the two register which were pushed onto stack
			mov    temp_char, al
									   //----------------- For the sake of arguemnt following discussion is done considering the string = "hello" and its encrypted string------------------            
										// for 1st iteration 23 is loaded into temp char and 23 in hex is 17

										// for 2nd temp char has value   204 or CC in hex

										// for 3rd iteration 7 is loaded into temp char

										// for 4thiteration  240 is loaded into temp char or CC in hex

										// for 5th iteration 19 is loaded into temp char or 13 in hex

			pop    edx                 //In the same way we are popping the stack and filling the entries 
			pop    ecx                 //of each register back into the respective register
			pop    eax                 //so we may not loose any data while entering or exitting the function
		}

		DChars[i] = temp_char;         // Store encrypted char in the Encrypted Chars array
	}
	return;
	__asm
	{
		dencrypt_4:
			push ebp                       // the base pointer is pushed onto stack to safely exit the function
			mov ebp, esp                   // the new base pointer is actually the stack pointer 

			sub esp, 8                     //say we are going to use 2 local variables although it is not needed
			push edi                       // we will be using edi and edx for temporary allocation of paramters
			push edx

											//working
			mov eax, [ebp + 8];            //Move value of parameter 1= char into EAX 
			mov edx, [ebp + 12];           // Move value of parameter 2=encrypt key into ESI



											//assigning parameters to local variables
			mov[ebp - 4], edx               //4 has Parameter
											//the paramter value was passed to the esi
			not byte ptr[eax]			    // doing the same as it was done in the encryption routine
			add   byte ptr[eax], 0x04	    // note that this variations in Ekey are not dependent on the Char as it is dependent on the 
										    //Ekey assigned 
			movzx edi, byte ptr[eax]		//now saving the result into edi temporaly

			sub dl, 0x04			 		//here we will do the opposite of encryption and there fore we 
											//subtracting 4 

			ror dl, 1                       // now rotating right to negate the effect of rotating left
			ror dl, 1                       //same here 
			xor edi, edx                    // reverse of xor is xor so no changes needed here 
			mov eax, edi                    // in cdec the return value is stored in eax so using that register 

			pop edx
			pop edi

			mov esp, ebp
			pop ebp
			ret
	}

}
//-------------------------------------------end of decrypt_chars function--------------------------------------------------------------------