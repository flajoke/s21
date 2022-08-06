#include <ncurses.h>
void X_Wall();
int main() {
  initscr();
  WINDOW *win = newwin(1800, 1000, 0, 0);
  noecho();
  raw();
  wtimeout(win, 100);

  int LeftSpace = 37, RightSpace = 37, TopSpace = 13;

  char f = 'g';

  char P1 = ' ', P2 = ' ', b = ' ';

  int P1Y = 23, P1Score = 0;
  int P2Y = 13, P2Score = 0;

  double BallDirectionY = 1;
  unsigned short BallDirectionX = 1;

  refresh();
  wrefresh(win);

  while (f != 'q') {
    werase(win);
    wprintw(win, "%d%80d\n", P1Score, P2Score);

    for (int i = 0; i <= 80; i++) {
      wprintw(win, "-");
    }
    wprintw(win, "\n");

    for (int i = 0; i <= 25; i++) {
      i++;
      if (i == TopSpace)
        b = '@';
      if ((i == P1Y) || (i == P1Y + 2) || (i == P1Y - 2))
        P1 = '#';
      if ((i == P2Y) || (i == P2Y + 2) || (i == P2Y - 2))
        P2 = '#';

      wprintw(win, "|%3c%*c%*c%3|\n", P1, LeftSpace, b, RightSpace, P2);

      P1 = ' ';
      P2 = ' ';
      b = ' ';
    }

    for (int i = 0; i <= 80; i++) {
      wprintw(win, "-");
    }
    wprintw(win, "\n");

    f = wgetch(win);

    if (f == 'a') {
      if (P1Y > 3) {
        P1Y -= 2;
      }
    }
    if (f == 'z') {
      if (P1Y < 23) {
        P1Y += 2;
      }
    }
    if (f == 'k') {
      if (P2Y > 3) {
        P2Y -= 2;
      }
    }
    if (f == 'm') {
      if (P2Y < 23) {
        P2Y += 2;
      }
    }

    if ((TopSpace == 1) || (TopSpace == 23)) {
      BallDirectionY *= -1;
    }
    TopSpace += BallDirectionY * 2;

    if ((LeftSpace <= 1) || (LeftSpace >= 73)) {
      BallDirectionX *= -1;
    }

    if (BallDirectionX == 1) {
      LeftSpace -= 1;
      RightSpace += 1;
    } else {
      LeftSpace += 1;
      RightSpace -= 1;
    }

    if (LeftSpace <= 1) {
      if (((TopSpace == P1Y) || (TopSpace == P1Y - 2) ||
           (TopSpace == P1Y + 2))) {
      } else {
        P2Score++;
        TopSpace = 13;
        LeftSpace += 40;
        RightSpace -= 40;
      }
    }
    if (LeftSpace >= 73) {
      if (((TopSpace == P2Y) || (TopSpace == P2Y - 2) ||
           (TopSpace == P2Y + 2))) {
      } else {
        P1Score++;
        TopSpace = 13;
        LeftSpace -= 40;
        RightSpace += 40;
      }
    }

    if (P1Score >= 25) {
      wprintw(win, "Player 1 win");
      break;
    }
    if (P2Score >= 25) {
      wprintw(win, "Player 2 win");
      break;
    }
  }

  wrefresh(win);
  getch();
  endwin();

  return 0;
}

