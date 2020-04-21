/*
	The Cardiff LHC needs a program to assign primary care physicians to patients living in the city of Cardiff.
	Specifically, the program must provide the following features:
	1. Inserting a new doctor. The function takes as input the surname, first name and serial number of a new doctor and inserts the doctor in the archive.
	2. Insertion of a new patient. The function takes as input the surname, the name and the National Insurance Number of a new patient and inserts the patient in the archive.
	3. Doctor to patient assignment. The function takes as input the doctor's serial number and the patient's National Insurance Number. The program displays the surname and first name of the doctor and the surname and first name of the patient and asks the user for confirmation of the data entered. If the user confirms the data entered, the patient is assigned to the doctor. Note that each doctor cannot have more than a certain number of patients fixed in advance (which is 500 for the Cardiff LHC).
	4. Viewing doctors. The feature displays the list of all doctors belonging to the Cardiff LHC.
	5. Patient record display. The function takes the patient's National Insurance Number as an input and displays the patient's data (surname and name) and the data of his general practitioner (surname and name).
 */

 /*
  * To change this license header, choose License Headers in Project Properties.
  * To change this template file, choose Tools | Templates
  * and open the template in the editor.
  */

 /*
  * File:   Management_software_LHC.c
  * Author: dtfabio96
  *
  * Created on 10 febbraio 2016, 12.11
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Costanti per la gestione delle stringhe
#define STRING_MAX_SIZE 50
#define SSN_SIZE 16
#define CODE_SIZE 10

// Costanti per la gestione degli archivi
#define DOCTORS_SIZE 50
#define PATIENTS_SIZE 500

// Numero massimo di pazienti per dottore
#define MAX_PATIENTS_BY_DOCTOR 100

// Codifica dell'esito di una funzione
#define OK 0
#define FULL_ARCHIVE -1
#define ITEM_NOT_FOUND -2
#define DOCTOR_NOT_FOUND -3
#define PATIENT_NOT_FOUND -4
#define NO_MORE_PATIENTS_ALLOWED -5

// Codifica voci del menu
#define INSERT_PATIENT 1
#define INSERT_DOCTOR 2
#define ASSIGN_DOCTOR_TO_PATIENT 3
#define SHOW_PATIENT_CARD 4
#define EXIT 0

// Struttura per la codifica di un dottore
typedef struct {
	char name[STRING_MAX_SIZE];
	char surname[STRING_MAX_SIZE];
	char code[STRING_MAX_SIZE];
	int num_assigned_patients; // Numero di pazienti assegnati ad un dottore
} doctor;

// Struttura per la codifica di un paziente
typedef struct {
	char name[STRING_MAX_SIZE];
	char surname[STRING_MAX_SIZE];
	char ssn[SSN_SIZE]; // Social Security Number (SSN) - Codice fiscale
	char doctor_code[CODE_SIZE]; // Codice del medico curante
} patient;

// Struttura usata per codificare l'archivio dei medici
typedef struct {
	doctor doctor_list[DOCTORS_SIZE]; // Array contenente i medici dell'LHC
	int size; // Dimensione logica dell'array
} doctors;

// Struttura usata per codificare l'archivio dei pazienti
typedef struct {
	patient patient_list[PATIENTS_SIZE]; // Array contenente i pazienti
	int size; // Dimensione logica dell'array
} patients;

// Struttura usata per la stampa della scheda di un paziente
typedef struct {
	char patient_name[STRING_MAX_SIZE];
	char patient_surname[STRING_MAX_SIZE];
	char doctor_name[STRING_MAX_SIZE];
	char doctor_surname[STRING_MAX_SIZE];
} patient_card;

// Funzioni per la gestione dei pazienti
void init_patient_archive(patients*); // Funzione utilizzata per inizializzare l'archivio
patient read_patient(); // Funzione utilizzata per leggere i dati di un paziente
int add_patient(patients*, patient); // Funzione utilizzata per aggiungere un paziente
int search_patient(patients, char[]); // Funzione utilizzata per ricercare un paziente dato il SSN
int assign_doctor_to_patient(doctors, patients*, char[], char[]); // Funzione per assegnare un medico ad un paziente data la matricola del medico e il SSN del paziente
int assign_doctor_to_patient_by_index(doctors, patients*, int, int); // Funzione per assegnare un medico ad un paziente dati gli indici del medico e del paziente
patient_card get_patient_card(patients, doctors, char[]); //Funzione utilizzata per ottenere la scheda di un paziente dato il suo SSN
void print_patient_card(patient_card); // Funzione utilizzata per la stampa della scheda di un paziente

// Funzioni per la gestione dei medici
void init_doctor_archive(doctors*); // Funzione utilizzata per inizializzare l'archivio
doctor read_doctor(); // Funzione utilizzata per leggere i dati di un medico
int add_doctor(doctors*, doctor); // Funzione utilizzata per aggiungere un medico
int search_doctor(doctors, char[]); // Funzione utilizzare per ricercare un medico data la sua matricola

int main(int argc, char** argv) {
	int choice, result, patient_index, doctor_index;
	patient new_patient;
	doctor new_doctor;
	patients patient_archive;
	doctors doctor_archive;
	char ssn_to_search[SSN_SIZE];
	char doctor_code_to_search[CODE_SIZE];
	char answer;
	patient_card card;

	// Inizializzazione archivi
	init_patient_archive(&patient_archive);
	init_doctor_archive(&doctor_archive);

	do {
		system("clear"); // MAC Users
		// system("cls"); // WIN Users
		printf("********** MANAGEMENT LHC **********\n\n");
		printf("1. Insert new patient.\n");
		printf("2. Insert new doctor.\n");
		printf("3. Patient assignment.\n");
		printf("4. View assigned doctor.\n");
		printf("0. Exit.\n\n");
		printf("Scelta: ");

		scanf("%d", &choice);

		switch (choice) {
		case INSERT_PATIENT:
			system("clear"); // MAC Users
			// system("cls"); // WIN Users

			new_patient = read_patient();
			result = add_patient(&patient_archive, new_patient);

			switch (result) {
			case OK:
				printf("Patient inserted correctly.\n");
				break;
			case FULL_ARCHIVE:
				printf("Patient archive full.\n");
				break;
			}

			system("read -n 1 -s -p \"Press a key to continue...\""); // MAC Users
			// system("PAUSE"); // WIN Users
			break;
		case INSERT_DOCTOR:
			system("clear"); // MAC Users
			// system("cls"); // WIN Users
			new_doctor = read_doctor();
			result = add_doctor(&doctor_archive, new_doctor);

			switch (result) {
			case OK:
				printf("Doctor inserted correctly.\n");
				break;
			case FULL_ARCHIVE:
				printf("Archivio medici pieno.\n");
				break;
			}

			system("read -n 1 -s -p \"Press a key to continue...\""); // MAC Users
			// system("PAUSE"); // WIN Users
			break;
		case ASSIGN_DOCTOR_TO_PATIENT:
			system("clear"); // MAC Users
			// system("cls"); // WIN Users

			// Lettura del SSN del cliente
			do {
				printf("Patient National Insurance Number: ");
				scanf("%s", ssn_to_search);

				patient_index = search_patient(patient_archive, ssn_to_search);
				if (patient_index == ITEM_NOT_FOUND)
					printf("Patient not present in the archive.\n");

			} while (patient_index == ITEM_NOT_FOUND);

			// Lettura del codice del medico
			do {
				printf("Medical code: ");
				scanf("%s", doctor_code_to_search);

				doctor_index = search_doctor(doctor_archive,
						doctor_code_to_search);
				if (doctor_index == ITEM_NOT_FOUND)
					printf("Doctor not present in the archive.\n");
			} while (doctor_index == ITEM_NOT_FOUND);

			// Esplosione codici e richiesta conferma
			printf("Do you want to assign the doctor %s %s to the patient %s %s (s to confirm, n to cancel)?\n",
					doctor_archive.doctor_list[doctor_index].name,
					doctor_archive.doctor_list[doctor_index].surname,
					patient_archive.patient_list[patient_index].name,
					patient_archive.patient_list[patient_index].surname);
			scanf("\n%c", &answer);

			if (answer == 's') {
				result = assign_doctor_to_patient_by_index(doctor_archive,
						&patient_archive, doctor_index, patient_index);

				switch (result) {
				case OK:
					printf("Doctor assigned successfully.\n");
					break;
				case NO_MORE_PATIENTS_ALLOWED:
					printf("The selected doctor has reached the maximum number of patients allowed.\n");
					break;

				}
			} else
				printf("Operation deleted.\n");

			system("read -n 1 -s -p \"Press a key to continue...\""); // MAC Users
			// system("PAUSE"); // WIN Users
			break;
		case SHOW_PATIENT_CARD:
			system("clear"); // MAC Users
			// system("cls"); // WIN Users

			printf("National Insurance Number: ");
			scanf("%s", ssn_to_search);

			card = get_patient_card(patient_archive, doctor_archive,
					ssn_to_search);

			print_patient_card(card);

			system("read -n 1 -s -p \"Press a key to continue...\""); // MAC Users
			// system("PAUSE"); // WIN Users
			break;
		case EXIT:
			system("clear"); // MAC Users
			// system("cls"); // WIN Users
			break;
		default:
			printf("Wrong choice!");
			system("read -n 1 -s -p \"Press a key to continue...\""); // MAC Users
			// system("PAUSE"); // WIN Users
		}
	} while (choice != EXIT);

	return (EXIT_SUCCESS);
}

// DEFINIZIONE FUNZIONI

void init_patient_archive(patients *_patients) {
	_patients->size = 0;
}

patient read_patient() {
	patient _new_patient;

	printf("Enter the Name: ");
	scanf("%s", _new_patient.name);
	printf("Enter the Surname: ");
	scanf("%s", _new_patient.surname);
	printf("Enter the National Insurance Number: ");
	scanf("%s", _new_patient.ssn);

	return _new_patient;
}

int add_patient(patients *_patients, patient _new_patient) {
	int index;
	index = _patients->size;

	if (index < PATIENTS_SIZE) {
		strcpy(_patients->patient_list[index].name, _new_patient.name);
		strcpy(_patients->patient_list[index].surname, _new_patient.surname);
		strcpy(_patients->patient_list[index].ssn, _-.ssn);
		_patients->size++;
		return OK;
	} else
		return FULL_ARCHIVE;
}

int search_patient(patients _patient, char _ssn_to_search[]) {
	int index, result;

	index = 0;
	while ((index < _patient.size)
			&& (strcmp(_ssn_to_search, _patient.patient_list[index].ssn) != 0)) {
		index++;
	}

	if (index >= _patient.size)
		return ITEM_NOT_FOUND;
	else
		return index;
}

int assign_doctor_to_patient(doctors _doctors, patients *_patients,
		char _doctor_code[], char _patient_ssn[]) {
	int doctor_index, patient_index;

	doctor_index = search_doctor(_doctors, _doctor_code);

	if (doctor_index == ITEM_NOT_FOUND) {
		return DOCTOR_NOT_FOUND;
	}

	patient_index = search_patient(*_patients, _patient_ssn);

	if (patient_index == ITEM_NOT_FOUND) {
		return PATIENT_NOT_FOUND;
	}

	return assign_doctor_to_patient_by_index(_doctors, _patients, doctor_index,
			patient_index);

}

int assign_doctor_to_patient_by_index(doctors _doctors, patients *_patients,
		int _doctor_index, int _patient_index) {
	if (_doctors.doctor_list[_doctor_index].num_assigned_patients < MAX_PATIENTS_BY_DOCTOR) {
		strcpy(_patients->patient_list[_patient_index].doctor_code, _doctors.doctor_list[_doctor_index].code);
		return OK;
	} else
		return NO_MORE_PATIENTS_ALLOWED;
}

patient_card get_patient_card(patients _patients, doctors _doctors,
		char _ssn_to_find[]) {
	int patient_index, doctor_index;
	patient_card card;

	patient_index = search_patient(_patients, _ssn_to_find);

	strcpy(card.patient_name, _patients.patient_list[patient_index].name);
	strcpy(card.patient_surname, _patients.patient_list[patient_index].surname);

	doctor_index = search_doctor(_doctors, _patients.patient_list[patient_index].doctor_code);

	strcpy(card.doctor_name, _doctors.doctor_list[doctor_index].name);
	strcpy(card.doctor_surname, _doctors.doctor_list[doctor_index].surname);

	return card;
}

void init_doctor_archive(doctors *_doctors) {
	_doctors->size = 0;
}

doctor read_doctor() {
	doctor _new_doctor;

	printf("Enter the Name: ");
	scanf("%s", _new_doctor.name);
	printf("Enter the Surname: ");
	scanf("%s", _new_doctor.surname);
	printf("Enter the National Insurance Number: ");
	scanf("%s", _new_doctor.code);

	return _new_doctor;
}

int add_doctor(doctors *_doctors, doctor _new_doctor) {
	int index;
	index = _doctors->size;

	if (index < DOCTORS_SIZE) {
		strcpy(_doctors->doctor_list[index].name, _new_doctor.name);
		strcpy(_doctors->doctor_list[index].surname, _new_doctor.surname);
		strcpy(_doctors->doctor_list[index].code, _new_doctor.code);
		_doctors->size++;
		return OK;
	} else
		return FULL_ARCHIVE;
}

int search_doctor(doctors _doctors, char _code_to_search[]) {
	int index, esito;

	index = 0;
	while ((index < _doctors.size)
			&& (strcmp(_code_to_search, _doctors.doctor_list[index].code) != 0)) {
		index++;
	}

	if (index >= _doctors.size)
		return ITEM_NOT_FOUND;
	else
		return index;
}

void print_patient_card(patient_card _card) {
	printf("Patient Name: %s\n", _card.patient_name);
	printf("Patient Surname: %s\n", _card.patient_surname);
	printf("Doctor Name: %s\n", _card.doctor_name);
	printf("Doctor Surname: %s\n", _card.doctor_surname);
}
