local ecs = require "ecs"

local function rand_flt(from, to)
	local maxNumber = 32767
    return from + (math.random(maxNumber) / maxNumber) * (to - from)
end

local function move(it)
    for pos, vel, ent in ecs.each(it) do
        pos.x = pos.x + vel.x * it.delta_time
        pos.y = pos.y + vel.y * it.delta_time
		pos.z = pos.z + vel.z * it.delta_time
    end
end

local function gravity(it)
    for pos, vel, grav, plane, ent in ecs.each(it) do
        local planeEpsilon = 0.1
		
		if plane.x * pos.x + plane.y * pos.y + plane.z * pos.z < plane.w + planeEpsilon then
			do return end
		end
		
		vel.x = vel.x + grav.x * it.delta_time
        vel.y = vel.y + grav.y * it.delta_time
		vel.z = vel.z + grav.z * it.delta_time
    end
end

local function FrictionSystem(it)
    for vel, friction, ent in ecs.each(it) do
        vel.x = vel.x - vel.x * friction.value * it.delta_time
		vel.y = vel.y - vel.y * friction.value * it.delta_time
		vel.z = vel.z - vel.z * friction.value * it.delta_time
    end
end

local function ShiverSystem(it)
    for pos, shiver, ent in ecs.each(it) do
        pos.x = pos.x + rand_flt(-shiver.value, shiver.value)
		pos.y = pos.y + rand_flt(-shiver.value, shiver.value)
		pos.z = pos.z + rand_flt(-shiver.value, shiver.value)
    end
end

local function BounceSystem(it)
    for pos, vel, plane, bounciness, ent in ecs.each(it) do
        local dotPos = plane.x * pos.x + plane.y * pos.y + plane.z * pos.z
		local dotVel = plane.x * vel.x + plane.y * vel.y + plane.z * vel.z
		
		if dotPos < plane.w then
			pos.x = pos.x - (dotPos - plane.w) * plane.x
			pos.y = pos.y - (dotPos - plane.w) * plane.y
			pos.z = pos.z - (dotPos - plane.w) * plane.z

			vel.x = vel.x - (1.0 + bounciness.value) * plane.x * dotVel
			vel.y = vel.y - (1.0 + bounciness.value) * plane.y * dotVel
			vel.z = vel.z - (1.0 + bounciness.value) * plane.z * dotVel
		end
    end
end

local function DieSystem(it)
	for bullet, ent in ecs.each(it) do
		bullet.deathtime = bullet.deathtime - it.delta_time
	end
end

local function HitSystem(it)
    local entities = {}
    for posa, spherea, flaga, enta in ecs.each(it) do
        table.insert(entities, {pos = posa, sphere = spherea, flag = flaga, ent = enta})
    end

    for i = 1, #entities do
        local posa = entities[i].pos
        local spherea = entities[i].sphere
        local enta = entities[i].ent

        for j = i + 1, #entities do
            local posb = entities[j].pos
            local sphereb = entities[j].sphere
            local entb = entities[j].ent

            local dx = posa.x - posb.x
            local dy = posa.y - posb.y
            local dz = posa.z - posb.z

            local sqDist = dx * dx + dy * dy + dz * dz
            local bound = spherea.radius + sphereb.radius
            local sqBound = bound * bound

			if sqDist < sqBound then
				spherea.hited = 1
				sphereb.hited = 1
			end
        end
    end
end

-- local function HitSystem(it)
--  	for pos1, sphere1, flag1, ent1 in ecs.each(it) do
--  		for pos2, sphere2, flag2, ent2 in ecs.each(it) do
-- 			if ent1 ~= ent2 then
-- 				local dx = pos1.x - pos2.x
-- 				local dy = pos1.y - pos2.y
-- 				local dz = pos1.z - pos2.z

-- 				local sqDist = dx * dx + dy * dy + dz * dz
-- 				local bound = sphere1.radius + sphere2.radius
--  				local sqBound = bound * bound

-- 				if sqDist < sqBound then
-- 					sphere1.hited = 1
-- 					sphere2.hited = 1
-- 				end
--  			end
--  		end
--  	end
-- end

ecs.system(move, "Move", ecs.OnUpdate, "Position, Velocity")
ecs.system(gravity, "grav", ecs.OnUpdate, "Position, Velocity, Gravity, BouncePlane")
ecs.system(FrictionSystem, "FrictionSystem", ecs.OnUpdate, "Velocity, FrictionAmount")
ecs.system(ShiverSystem, "ShiverSystem", ecs.OnUpdate, "Position, ShiverAmount")
ecs.system(BounceSystem, "BounceSystem", ecs.OnUpdate, "Position, Velocity, BouncePlane, Bounciness")
ecs.system(DieSystem, "DieSystem", ecs.OnUpdate, "Bullet")
ecs.system(HitSystem, "HitSystem", ecs.OnUpdate, "Position, HitSphere, OriginFlag")
