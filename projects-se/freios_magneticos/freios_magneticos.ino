    //Sensor de velocidade em frenagem magnética
    
    #include <LiquidCrystal.h>
    
    //pinos utilizados
    LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // 12 RS - 11 E
    
    // analogico 
    int ldrInicial = 0;
    int ldrFinal1 = 1;
    int ldrFinal2 = 2;
    int pot = 3;
    int Piezo = 4;
    
    //digital
    int button1 = 9;  // sem definição de quantidade de teste
    int button2 = 8;  // com definição
    int buzzer = 7;
        
    // variaveis 
    int ldrValorIncial = 0;
    int ldrValorFinal1 = 0;
    int ldrValorFinal2 = 0;
    int tempo = 0;
    int vet_temp[20];
    int valorVetor = 5;
    float d = 45; // distancia do tubo
    int but1 = 0;  // botão 1 
    int but2 = 0;  // botão 2
    int quantidade = 0;
    int byteEntrada = 0;
    int pressaodefinida = 1000; // Valor do gatilho que irá dizer se houve ou não toque
    int valorPiezo = 0; //
        
    void setup() {
      Serial.begin(9600);     //inicia a comunicação serial
      lcd.begin(16, 2);       //configura numero de linhas e colunas
      pinMode(buzzer, OUTPUT);  //define a porta 7 como saída
      pinMode(button1 , INPUT);
      pinMode(button2 , INPUT);
      noTone(buzzer);
      lcd.print("Sensor velocidade");
    }
    
    void loop() {
      lcd.setCursor(0,0);            // posição inicial do cursor
      int i = 0;
      but1 = digitalRead(button1);
      but2 = digitalRead(button2);
      
      if(Serial.available()>0){ // execução através da interface grafica Java
        byteEntrada = Serial.read();
        calcular_tempo();
        if(byteEntrada == 1){
           calcular_tempo();
        }
      }
        
      if(but1 == HIGH){ // execução modo predefinido pelos programadores
        lcd.print("select mod1:");
        do{
          vet_temp[i] = calcular_tempo();
          i++;
          Serial.println(i);
        }while(but1 == HIGH && i < valorVetor);
          calcular();
      }
    
      if(but2 == HIGH){ // execução definida pelo usuário, a partir do potenciometro
           Serial.println("botao 2");
           delay(100);
        do{    
          lcd.clear();
          quantidade = (analogRead(pot)/50);
          lcd.print(quantidade);
          Serial.println(quantidade);
          delay(100);
          but2 = digitalRead(button2);
          
        }while(but2 == LOW);
         
         Serial.println("selecionado");
         Serial.println(quantidade);
         lcd.clear();
         lcd.print("select mod2 : ");
         lcd.print(quantidade);
        do{
          vet_temp[i]= calcular_tempo();
          i++;
          pressaodefinida = 1024; // Valor do gatilho que irá dizer se houve ou não toque
          valorPiezo = 0;
        }while(quantidade>i);
        
        calcular();
      }
    }

    void calcular(){ // executa os calculos da media, variancia e desvio padrão
      int j=0;
      float soma = 0;
      while(vet_temp[j]>0 && j<valorVetor){
        Serial.println(vet_temp[j]);
        soma = soma+((float)vet_temp[j]);
        j++;
        
      }
      soma = soma/100;
       
      float media = 0;
      float variancia = 0;
      float padrao = 0;
      if(j>0){
        media = (soma/(j));
      }
      if( j > 1){
        for(int i=0; j>i ;i++){
           variancia = pow((((float)vet_temp[i]) - media),2) + variancia;
        }
        variancia = (variancia/(j-1));
        padrao = sqrt(variancia);
      }
      
      lcd.clear();
      lcd.print("m:");
      lcd.print(media);
      lcd.print("s");
      lcd.print("   v:");
      lcd.setCursor(0,1);
      lcd.print(variancia);
      lcd.print(" dp:");
      lcd.print(padrao);
      
    }
    
    int calcular_tempo(){ // calcula o tempo de queda do objeto
      tempo=0;
      while(ldrValorIncial<200){
      ldrValorIncial = analogRead(ldrInicial);   //obtem o valor lido pelo sensor inicial
       if (ldrValorIncial >= 200){
        tone(buzzer,1500);  
        lcd.print(ldrValorIncial);
        do{
          ldrValorFinal1 = analogRead( ldrFinal1);  
          ldrValorFinal2 = analogRead( ldrFinal2);
          lcd.print(tempo);
          tempo = tempo + 1;
          delay(10);
          lcd.clear();
        } while( ldrValorFinal1 <= 100 || ldrValorFinal2 <= 100);
        noTone(buzzer);
     
        lcd.print(tempo);
        ldrValorIncial = 0;
        Serial.print("t");
        Serial.print(tempo);
        piezoeletrico();
        // Serial.print("p");
        // Serial.println(pressaodefinida);
        //pressaodefinida = 1000;
        //valorPiezo = 0;
        return tempo; 
        }
      }
    }
    
    int piezoeletrico(){
      int aux = 40;
      while(aux!=0){
        valorPiezo = analogRead(Piezo);
        if (valorPiezo < pressaodefinida) {
          pressaodefinida = valorPiezo;
        }
        delay(10);
        aux--;    
      };
      Serial.print("p");
      Serial.println(pressaodefinida);
    }
