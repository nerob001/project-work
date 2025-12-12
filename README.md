# 🎮 Catch The Circle

A fast-paced SFML game where fruits fall from the sky and the player must catch the positive ones using a basket.  
Avoid negative (white) fruits, keep your lives above zero, and aim for the highest score!

---

## 📝 Game Description

- You control a basket at the bottom of the screen.  
- Colorful fruits fall randomly from above.  
- **Positive fruits** → +10 points  
- **Negative fruits (white)** → -10 points  
- Missing a positive fruit → lose **1 life**  
- Start with **5 lives**  
- Game ends when lives reach **0**  
- Press **R** to restart after Game Over

Difficulty increases gradually over time.

---

## Screenshots


## Controls
- Left Arrow: Move left
- Right Arrow: Move right
- Esc → Exit game
- R → Restart (when Game Over)

---

## 🛠️ Setup Requirements
- **C++17** or newer  
- **SFML 2.5+**  
- Compiler: g++ / clang / MSVC

---

##Important
- Need a .vs file for Visual Studio

---

## How to Build and Run
### g++ (Linux)
g++ -std=c++17 main.cpp -o CatchTheCircle \
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

./CatchTheCircle

---


  
