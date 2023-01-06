
require 'rsc/lua/lib'

-- My little brother Kellen says it looks better if it's desaturated.
-- Doing this by setting it to 60% black&white by default
-- hes right 
NORMAL_BLACK_WHITE = 0.6

-- function level1()
--     add_custom_scene(function()
--         _clear()
--         _delay(1.0)
--         _title("Level 1")
--         _delay(2.0)
--     end)

--     -- add_custom_scene(function()
--     --     local image = _load("image3.png")
--     --     _clear()
--     --     _delay(1)
--     --     _title("Look out")
--     --     _delay(2.5)
--     --     _clear()
--     --     _subtitle("don't look behind you")
--     --     _delay(4.7)
--     --     _clear()
--     --     _delay(7.4)
--     --     _subtitle("test start")
--     --     _delay(0.7)
--     --     _show_image_left(image)
--     --     _delay(1)
--     --     _unload(image)
--     -- end)

--     -- if math.random(0,2) == 0 then
--     --     add_custom_scene(function()
--     --         _clear()
--     --         _delay(1)
--     --         _subtitle("Run")
--     --         _play_sound("rsc/sounds/background.wav")
--     --         _delay(6.7)
--     --         _clear()
--     --         _delay(3)
--     --     end)
--     -- end

--     for i=1,math.random(1,2) do add_guess_scene("image") end

--     add_guess_scene("audio")

--     add_custom_scene(function()
--         _clear()
--         _delay(1.0)
--         _title("End")
--         _delay(2.0)
--         _clear()
--         _subtitle("Correct: " .. right_count)
--         _delay(2)
--         _clear()
--         _subtitle("Incorrect: " .. wrong_count)
--         if (wrong_count > right_count) then
--             _play_sound("rsc/sounds/static-in.wav")
--             _delay(2)
--         end
--         _delay(2)
--         _clear()
--         _delay(1.0)
--     end)
--     run_scenes()
-- end

-- function level2()
--     add_custom_scene(function()
--         print "level2!"
--     end)
--     for i=1,3 do add_guess_scene("image") end
--     run_scenes()
-- end



--uncomment everything below this for any release builds
function intro()
    -- add_custom_scene(function()
    --     local wait_time = 1.5
    --     _clear()
    --     _delay(1)
    --     _title("Click to begin.")
    --     _wait_for_selection()
    --     _clear()
    --     _add_line("This video is property of the FDD.", 0)
    --     _delay(wait_time)
    --     _add_line("The unlawful reproduction or", 1)
    --     _delay(wait_time)
    --     _add_line("distribution of this training exercise", 2)
    --     _delay(wait_time)
    --     _add_line("is a federal offense punishable by", 3)
    --     _delay(wait_time)
    --     _add_line("lifetime imprisonment", 4)
    --     _delay(2)
    --     _add_line("or", 6)
    --     _delay(2.5)
    --     _add_line("death.", 8)
    --     _play_and_wait_sound("rsc/sounds/static-in.wav")
    -- end)

    add_custom_scene(function()
        local wait_time = 1.5
        _clear()
        _delay(wait_time)
        
    end)
    run_scenes()
end


function part1()

    add_custom_scene(function()
        _clear()
        _delay(1.0)
        _title("Level 1")
        _delay(2.0)
    end)

    for i=1,5 do 
        add_guess_scene("image", 3)
    end
    for i=1,5 do
        if math.random(4) ~= 4 then
            add_guess_scene("image", 2)
        else
            add_guess_scene("image", 4)
        end
    end
    for i=1,5 do
        if math.random(5) ~= 5 then
            add_guess_scene("image", 1)
        else 
            add_guess_scene("image", 5)
        end
    end
    run_scenes()
end

levels = {}
levels[1] = intro
levels[2] = part1
-- levels[3] = part2

function main()
    save = _load_save("save.dat")

    level = _get_level(save)
    print("Got save. level = " .. level)
    _set_level(save, level + 1)
    print("Moved to next level. level = " .. _get_level(save))

    for i=0,6 do
        print("Creepy flag[" .. i .. "] = " .. tostring(_get_creepy_flag(save, i)))
        _set_creepy_flag(save, i, math.random() < 0.5)
    end

    print("The current fear factor is " .. _get_fearfactor(save));
    _set_fearfactor(save, math.random())
    print("The new fear factor is " .. _get_fearfactor(save));

    _store_save(save, "save.dat")
    _close_save(save)

    _set_bw(NORMAL_BLACK_WHITE)
    for i = 1, #levels do
        levels[i]()
    end
end
