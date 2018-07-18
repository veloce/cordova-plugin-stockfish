#include "threadbuf.h"
#include "uci.h"
#include "bitboard.h"
#include "position.h"
#include "search.h"
#include "evaluate.h"
#include "pawns.h"
#include "thread.h"
#include "tt.h"
#include "tbprobe.h"
#include "CordovaPluginStockfishc.h"

namespace PSQT {
  void init();
}

namespace stockfishios
{
  static std::string CMD_EXIT = "stockfish:exit";

  auto readstdout = [](void *stockfish) {
    std::streambuf* out = std::cout.rdbuf();

    threadbuf lichbuf(8, 8096);
    std::ostream lichout(&lichbuf);
    std::cout.rdbuf(lichout.rdbuf());
    std::istream lichin(&lichbuf);

    std::string o = "";

    while(o != CMD_EXIT) {
      std::string line;
      std::getline(lichin, line);
      if(line != CMD_EXIT) {
        const char* coutput = line.c_str();
        StockfishSendOutput(stockfish, coutput);
      } else {
        o = CMD_EXIT;
      }
    };

    // Restore output standard
    std::cout.rdbuf(out);

    lichbuf.close();
  };

  std::thread reader;

  void init(void *stockfish) {
    reader = std::thread(readstdout, stockfish);

    UCI::init(Options);
    PSQT::init();
    Bitboards::init();
    Position::init();
    Bitbases::init();
    Search::init();
    Pawns::init();
    Tablebases::init(CHESS_VARIANT, Options["SyzygyPath"]); // After Bitboards are set
    Threads.set(Options["Threads"]);
    Search::clear(); // After threads are up
    TT.resize(Options["Hash"]);
  }

  void cmd(std::string cmd) {
    UCI::command(cmd);
  }

  void exit() {
    sync_cout << CMD_EXIT << sync_endl;
    reader.join();
    Threads.set(0);
  }
}
