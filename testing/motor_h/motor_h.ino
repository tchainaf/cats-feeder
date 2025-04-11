//Programa: Controle de motor DC com L293D
//Autor: Saulo Alexandre 


int PinoVelocidade = 3; //Pino que define a velocidade do motor e está ligado ao pino 1 do L293D e a Digital 3 do Arduino 
int Entrada1 = 2; //Entrada do terminal do motor ligado ao pino 2 do L293D e a Digital 2 do Arduino 
int Entrada2 = 4; //Entrada do terminal do motor ligado ao pino 7 do L293D e a Digital 7 do Arduino 

void setup( ) 
{ 
    //Iniciando o Serial do arduino 
    Serial.begin(9600); 
    Serial.println("Vamos la controlar o motor:"); 
    Serial.println("Digite 1 para ir para frente"); 
    Serial.println("Digite 2 para ir para tras"); 
    Serial.println("Digite 0 para parar"); 
    //Define os pinos como saida 
    pinMode(PinoVelocidade, OUTPUT); 
    pinMode(Entrada1, OUTPUT); 
    pinMode(Entrada2, OUTPUT); 

} 

void loop( ) 
{ 

    //Define a velocidade de rotacao 
    int velocidade = 1000; 
    analogWrite(PinoVelocidade, velocidade); 
    //Verificando se algum valor foi inserido no Serial 
    if(Serial.available() > 0) { 
        //Convertendo esse valor para Inteiro e atribuindo a variável val 
        int val = Serial.parseInt(); 
        //definindo o movimento do motor de acordo com o valor digitado 
        if (val == 1) { 
            digitalWrite(Entrada1, HIGH); 
            digitalWrite(Entrada2, LOW); 
            Serial.println("Girando para Frente"); 
        } else if (val == 2) { 
            digitalWrite(Entrada1, LOW); 
            digitalWrite(Entrada2, HIGH); 
            Serial.println("Girando para Tras"); 
        } else if (val == 0) { 
            digitalWrite(Entrada1, LOW); 
            digitalWrite(Entrada2, LOW); 
            Serial.println("Parado"); 
        } else { 
            Serial.println("Opcao Invalida"); 
            Serial.println("Digite 0, 1 ou 2"); 
        }     

        //dando um intervalo para evitar mudança de estado brusca
        delay (100);
    } 
}