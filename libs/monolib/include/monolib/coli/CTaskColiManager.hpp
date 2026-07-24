#pragma once

/**
 * CTaskColiManager — Collision task manager.
 *
 * Manages collision processing as a process task.
 * Inherits from CTTask<CTaskColiManager> (CRTP) for Move/Draw dispatch.
 *
 * Full class definition is in the owning translation unit to avoid
 * inline-method collisions from CTTask.hpp explicit instantiations.
 */

class CTaskColiManager;
