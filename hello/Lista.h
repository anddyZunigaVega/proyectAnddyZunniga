#pragma once  
#include "iostream"  
#include "string"  
#include "Ranking.h"  
using namespace std;  


class Lista:public Ranking{  

 struct Nodo {  
	Ranking* dato;  
	Nodo* siguiente;  
	Nodo* anterior;  

  Nodo(Ranking* nDato) : dato(nDato), siguiente(nullptr), anterior(nullptr) {}  

 };  
 Nodo* head;  
 public: 
 Lista() : head(nullptr) {}
 Lista(Ranking* nDato) : head(new Nodo(nDato)){}

 Nodo* getHead() const { return head; }

 void agregarOrdenado(Ranking* nDato) {
	 Nodo* nuevoNodo = new Nodo(nDato);
	 if (!head) {
		 head = nuevoNodo;
		 return;
	 }

	 Nodo* actual = head;
	 Nodo* anterior = nullptr;

	 while (actual && nDato->getScoreForMoves() < actual->dato->getScoreForMoves()) {
		 anterior = actual;
		 actual = actual->siguiente;
	 }
	 if (!anterior) {
		 nuevoNodo->siguiente = head;
		 head->anterior = nuevoNodo;
		 head = nuevoNodo;
	 }
	 else {
		 nuevoNodo->siguiente = actual;
		 nuevoNodo->anterior = anterior;
		 anterior->siguiente = nuevoNodo;
		 if (actual) {
			 actual->anterior = nuevoNodo;
		 }
	 }
 }

 string getPrimerosDiez() {
	 string resultado;
	 Nodo* actual = head;
	 int contador = 1;
	 while (actual && contador <= 10) {

		 resultado += to_string(contador) + ". " + actual->dato->getName() + " - Score: " + to_string(actual->dato->getScore()) + " - Moves: " + to_string(actual->dato->getMoves()) + "\n";
		 
		 actual = actual->siguiente;

		 contador++;
	 }
	 return resultado;
 }

 ~Lista() {
	 Nodo* actual = head;
	 while (actual) {
		 Nodo* siguiente = actual->siguiente;
		 delete actual->dato;
		 delete actual;
		 actual = siguiente;
	 }
 }
};