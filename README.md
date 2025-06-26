
# 🐦 Save the Bird

A third-person DirectX game where you must find and rescue a bird by defeating all enemies before time runs out.

---

## 🎮 Controls

- `T` — Switch camera view  
- `W A S D` — Movement  
- `Space` — Jump  
- `Mouse` — Rotate view  
- `Left Mouse Button` — Shoot  
- `Enter` — Advance dialogue / Confirm menu  
- `Escape` — Quit the game at any point  

---

## 📸 Screenshots

<div style="display: flex; justify-content: center;"> 
  <img src="https://github.com/UWEGames-GiC2/directx-game-MonikaBuk/assets/115149820/c6037f37-e493-4a1d-9927-e2afbf89265e" width="400" alt="Gameplay Screenshot 1"> 
  <img src="https://github.com/UWEGames-GiC2/directx-game-MonikaBuk/assets/115149820/f6d094a6-e445-4069-b105-e0ca9a0b9b6f" width="400" alt="Gameplay Screenshot 2"> 
</div>

---

## 🎥 Demo

[![YouTube Video](https://github.com/UWEGames-GiC2/directx-game-MonikaBuk/assets/115149820/8f2e723a-de4a-4ac5-9b4e-b353d604afb1)](https://youtu.be/njV2Zi2jEMM)

---

## 🧠 About the Project

This game uses a tile-based map system defined in a text file, where numbers represent different objects:
- `1` = Wall  
- `2` = Enemy  
- `4` = Hostage (Bird)  
- `0` = Walkable tile  

The map is dynamically generated from this file, allowing easy editing and scaling. Enemy count and placement can be changed by editing the map file and updating the row/column dimensions.

### 🔍 AI & Pathfinding
- A **Breadth-First Search** algorithm is used for enemy pathfinding.
- Enemies detect the player at a certain distance, and when close enough, they will **chase and shoot**.

### 🧱 Tilemap
- Built using a text file input and renders with DirectX.
- Easily expandable to larger or more complex maps.

### 🗣️ Dialogue System
- Features a simple interactive **dialogue system** with the bird (hostage).
- Game time pauses during dialogues.
- If all enemies aren't defeated, the bird prompts the player to continue fighting before entering the win state.

### 🧨 Combat & Loss Conditions
- Players can **lose** by:
  - Running out of time
  - Taking too much damage from enemies

### 🧊 Animation
- Weapon uses a 2D animation system.
- A 3D animation system was attempted: bones are loaded and stored, but position updates currently don't function as intended.

![Weapon animation debug](https://github.com/UWEGames-GiC2/directx-game-MonikaBuk/assets/115149820/656f1108-73f8-489d-8e6f-529cf736533c)

---

## 📚 References

- LEGO Han Solo Model – [Free3D.com](https://free3d.com/3d-model/lego-han-solo-37069.html)

---

## 🧑‍💻 Tech Stack

- **Language:** C++  
- **Graphics API:** DirectX  
- **Custom engine logic**: Tilemap, dialogue system, AI, basic animation

---

## 🙋‍♀️ Author

Your Name – [@yourhandle](https://github.com/yourhandle)

---

## 📄 License

This project is licensed under the MIT License.


‌
