#ifndef SOAK_OVERFLOW_ACTION_SHOOT_H
#define SOAK_OVERFLOW_ACTION_SHOOT_H

namespace soak_overflow::action
{
    class Shoot
    {
    // constructor *************************************************************
    public:
        Shoot(int target_id);

        Shoot(const Shoot& cr_shoot) = default;

        Shoot(Shoot&& rv_shoot) noexcept = default;

        ~Shoot() noexcept = default;

    // operator ****************************************************************
    public:
        Shoot& operator=(const Shoot& cr_shoot) = default;
        
        Shoot& operator=(Shoot& rv_shoot) noexcept = default;

    // target ******************************************************************
    public:
        int get_target_id() const;

    private:
        int __target_id;
    };
}

#endif
