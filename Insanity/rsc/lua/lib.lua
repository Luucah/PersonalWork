
local scene_queue = {}
local creepy_flags = {false, false, false, false}



total_questions = 0
right_count = 0
wrong_count = 0
fear_factor = 0


-- Bindings that Aidan needs to write in CPP
function _start_menu() print "[C++] Make Start Menu" end
function _set_bg_color() print "[C++] Make Background Color Changer" end

function reset_count()
    right_count = 0
    wrong_count = 0
    total_questions = 0
end

local function add_to_queue(callback, kind, difficulty)
    scene_queue[#scene_queue+1] = {callback, kind, difficulty}
end

local function get_random_sample(used, kind, difficulty)

    name = ""
    if kind == "image" then
        repeat
            name = "rsc/images/" .. difficulty .. "/" .. math.random(2) -- change to actual number of folders in difficulty
        until(used[name] == nil)
        print("Displaying: " .. name)
        return name
    elseif kind == "audio" then
        repeat
            name = "rsc/sounds/" .. difficulty .. "/" .. math.random(2) -- change to actual number of folders in difficulty
        until(used[name] == nil)
        print("Playing: " .. name)
        return name
    end
end

--kind parameter is just passed to random sample function
local function get_samples(kind, difficulty)
    local used = {}
    local samples = {}
    local sample = get_random_sample(used, kind, difficulty)
    used[sample] = true
    samples[1] = sample .. "/good"
    samples[2] = sample .. "/bad"
    return samples
end

local function guess_scene(kind, difficulty) 
    print("----- Guess Scene: " .. kind)

    total_questions = total_questions + 1
    print("----- Question number: " .. total_questions)
    -- Grab two samples
    local samples = get_samples(kind, difficulty)
    -- samples[1] is the correct choice

    local image1 = nil
    local image2 = nil
    local sound1 = nil
    local sound2 = nil
    if kind == "image" then
        image1 = samples[1] .. ".png"
        image2 = samples[2] .. ".png"
        -- just beep sound because no necessary audio
        sound1 = "rsc/sounds/beep.wav"
        sound2 = "rsc/sounds/beep.wav"
    elseif kind == "audio" then
        -- just default image because user is meant to focus on sound
        image1 = "rsc/images/soundimageL.png"
        image2 = "rsc/images/soundimageR.png"
        sound1 = samples[1] .. ".wav"
        sound2 = samples[2] .. ".wav"
    end

    local left = image1
    local right = image2
    local leftsound = sound1
    local rightsound = sound2
    local correct = 1

    if math.random(0,2) == 0 then
        if kind == "image" then left, right = image2, image1 end
        leftsound = sound2
        rightsound = sound1
        correct = 2
    end

    -- Do the sequence
    _clear()
    _delay(1.0)
    _show_image_left(left)
    if kind == "image" then _play_sound(leftsound) else _play_and_wait_sound(leftsound) end
    _delay(1.7)
    _show_image_right(right)
    if kind == "image" then _play_sound(rightsound) else _play_and_wait_sound(rightsound) end
    _delay(0.3)
    _subtitle("Make your selection")
    _delay(0.3)
    local selection = _wait_for_selection()
    _delay(0.3)
    _clear()
    _delay(1.4)
    if selection == correct then
        _title("Correct.")
        _delay(1.8)
        right_count=right_count+1
    else
        _delay(0.4)
        _title("Wrong.")
        _delay(2.2)
        wrong_count=wrong_count+1
    end
    _clear()
    _delay(1.2)

    -- decide whether to make the game creepier based on how many questions have been answered
    -- fear factor can only be 2 for every 5 questions answered. probably. unless i fucked up an inequality
    if total_questions >= 5 and fear_factor <= (total_questions//5) * 2 then
        local remainder = total_questions%5 
        if math.random(remainder) == 1 then
            fear_factor = fear_factor + 1
            print("FEAR FACTOR INCREASED BY ONE, CURRENTLY: " .. fear_factor)
        end
    end
end

function add_guess_scene(kind, difficulty)
    add_to_queue(guess_scene, kind, difficulty)
end

function add_custom_scene(callback)
    add_to_queue(function() 
        print "----- Custom Scene"
        callback()
    end, nil)
end

function run_scenes(callback)
    for i = 1, #scene_queue do
        scene_queue[i][1](scene_queue[i][2], scene_queue[i][3])
        --add a check for performance, increment creepy value
    end
    scene_queue = {}
end
