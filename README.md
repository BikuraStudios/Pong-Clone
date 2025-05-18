Pong (SFML + C++)

A rough interpretation of Harvard's CS50 introduction to game development module 0.

Controls:

| Player | Action    | Key       |
| ------ | --------- | --------- |
| 1      | Move Up   | `W`       |
| 1      | Move Down | `S`       |
| 2      | Move Up   | `↑ Arrow` |
| 2      | Move Down | `↓ Arrow` |
| All    | Start Game| `Enter`   |
| All    | Quit Game | `Escape`  |
| All    | Pause Game| `Space`   |
| All    | Color Swap| `Tab`     |

Known Issues:

Ball behaves erraticly when hitting paddles on corners.
Window Resize / Movement breaks internal physics.

Todo:
Write up coding post-mortem.

Dependencies:
SFML

Note:
Fixing this code is probably beyond my current abilities. I'll come back to this project in a few months as practice for cleaning up and refactoring.
