#include <LiquidCrystal.h>

// LCD wiring and contrast setup
int Contrast = 75;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Joystick pins
const int yPin = A0;        // Vertical movement (VRy)
const int buttonPin = 7;    // Fire button
const int buzzerPin = 8;    // Buzzer pin

// Game variables
int playerPos = 1;          // Vertical player position (0 or 1 for LCD rows)
int bulletPosX = -1;        // Bullet horizontal position (-1 means no bullet)
int bulletPosY = -1;        // Bullet vertical position
int enemyPos = 15;          // Horizontal enemy position
int enemyRow = 0;           // Enemy row (0 or 1)

bool bulletFired = false;
bool gameOver = false;

unsigned long lastFireTime = 0;  // Last time a bullet was fired
const unsigned long fireCooldown = 300;  // Cooldown time in milliseconds

// Scoring variables
int score = 0;              // Player's current score

// Enemy speed variables
unsigned long lastEnemyMoveTime = 0; // Time of the last enemy movement
unsigned long enemySpeed = 400;     // Initial enemy movement speed (in milliseconds)
const unsigned long minEnemySpeed = 100; // Minimum speed for enemy movement
const unsigned long speedIncreaseInterval = 5000; // Interval to increase enemy speed
unsigned long lastSpeedIncreaseTime = 0;

int melody[] = {
  330, 330, 349, 392, 392, 349, 330, 294,
  262, 262, 294, 330, 330, 294, 294, 330,
  330, 349, 392, 392, 349, 330, 294, 262,
  262, 294, 330, 294, 262, 262,
};

int noteDurations[] = {
  4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 2, 4,
  4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 2, 8, 2, 
};

void setup() {
  // Set up LCD and contrast
  analogWrite(6, Contrast);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Space Invaders!");
  delay(2000);
  lcd.clear();

  // Play the startup sound (Ode to Joy)
  playStartUpMelody();

  // Joystick and buzzer setup
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
}

// Function to play the startup melody
void playStartUpMelody() {
  for (int thisNote = 0; thisNote < 31; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);

    // Wait for the note to finish before playing the next note
    int pauseBetweenNotes = noteDuration * 1.30; // Adding a small gap between notes
    delay(pauseBetweenNotes);

    // Stop the tone after each note
    noTone(buzzerPin);
  }
}

// Function to draw the game screen
void drawScreen() {
  lcd.clear();

  // Draw the enemy
  lcd.setCursor(enemyPos, enemyRow);
  lcd.write('X');

  // Draw the bullet
  if (bulletFired && bulletPosX != -1) {
    lcd.setCursor(bulletPosX, bulletPosY);
    lcd.write('-');
  }

  // Draw the player spaceship
  lcd.setCursor(0, playerPos);
  lcd.write('>');

}

// Function to move the enemy
void moveEnemy() {
  unsigned long currentTime = millis();

  if (currentTime - lastEnemyMoveTime >= enemySpeed) {
    enemyPos--;
    lastEnemyMoveTime = currentTime;

    // Reset enemy position if it goes off-screen
    if (enemyPos < 0) {
      enemyPos = 15;
      enemyRow = (enemyRow + 1) % 2; // Alternate row
    }
  }
}

// Function to handle bullet movement and collision
void updateBullet() {
  if (bulletFired) {
    bulletPosX++; // Move bullet to the right
    if (bulletPosX > 15) {
      bulletFired = false; // Bullet goes off-screen
      bulletPosX = -1;
      bulletPosY = -1;
    } else if (bulletPosX == enemyPos && bulletPosY == enemyRow) {
      // Bullet hits the enemy
      bulletFired = false;
      bulletPosX = -1;
      bulletPosY = -1;
      score += 100; // Increase score by 100 points
      enemyPos = 15; // Reset enemy position
      enemyRow = (enemyRow + 1) % 2; // Alternate row

      // Play hit sound
      tone(buzzerPin, 1000, 200); // 1 kHz tone for 200 ms
    }
  }
}

// Function to increase enemy speed over time
void increaseEnemySpeed() {
  unsigned long currentTime = millis();

  if (currentTime - lastSpeedIncreaseTime >= speedIncreaseInterval) {
    lastSpeedIncreaseTime = currentTime;
    if (enemySpeed > minEnemySpeed) {
      enemySpeed -= 10; // Increase speed by reducing delay
    }
  }
}

// Function to check for joystick input
void checkInput() {
  int yValue = analogRead(yPin);
  bool buttonPressed = digitalRead(buttonPin) == LOW;
  unsigned long currentTime = millis();

  // Move player up or down
  if (yValue < 400 && playerPos > 0) {
    playerPos--;
  } else if (yValue > 600 && playerPos < 1) {
    playerPos++;
  }

  // Fire bullet (with cooldown)
  if (buttonPressed && currentTime - lastFireTime > fireCooldown) {
    bulletFired = true;
    bulletPosX = 1;      // Bullet starts right next to the spaceship
    bulletPosY = playerPos;
    lastFireTime = currentTime;  // Update last fire time

    // Play fire sound
    tone(buzzerPin, 1500, 100); // 1.5 kHz tone for 100 ms
  }
}

// Main loop
void loop() {
  if (!gameOver) {
    checkInput();
    updateBullet();
    moveEnemy();
    increaseEnemySpeed();

    // Check if the enemy reaches the spaceship
    if (enemyPos == 0 && enemyRow == playerPos) {
      gameOver = true;

      // Play game-over sound
      for (int i = 0; i < 3; i++) {
        tone(buzzerPin, 500, 300); // 500 Hz tone for 300 ms
        delay(400);
      }

      lcd.clear();
      lcd.print("Game Over!");
      lcd.setCursor(0, 1);
      lcd.print("Score:");
      lcd.print(score);
      digitalWrite(buzzerPin, LOW); // Turn buzzer off
      return;
    }

    drawScreen();
    delay(50); // Frame update speed
  }
}
