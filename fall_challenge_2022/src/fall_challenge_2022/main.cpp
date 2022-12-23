#include <iostream>

#include <fall_challenge_2022/State.h>

int main()
{
    int width;
    int height;

    std::cin >> width >> height;
    std::cin.ignore();

    fall_challenge_2022::State state(width, height);

    while(true)
    {
        state.update_from_std_in();

        std::cerr << state.get_map().to_string() << std::endl;
        std::cout << "WAIT" << std::endl;
    }
}
