#include "../lib8085.h"
#include "../assembler.h"

namespace retro85
{
    class App
    {
        public:
            App();

            bool assemble(std::string& code);
            bool step();
            bool pause();
            bool reset();

            const std::map<uint64_t, std::string>& get_disassembly();
            lib8085::Processor* get_cpu();

            ~App();
        private:
            lib8085::Assembler _assembler;
            lib8085::Processor _cpu;

            int m_width;
            int m_height;
    };
}
