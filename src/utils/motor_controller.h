#pragma once

#include <memory>

#include "odrive_socket.h"
#include "estop.h"

class MotorController : public Estop
{
private:
    std::shared_ptr<ODriveSocket> _odrv_socket;
    canid_t _motor_id;
    float _torque_constant = 8.27F / 330.0F;

    void estop(int sig) override {
        printf("Running ESTOP\n");
        _odrv_socket->setAxisState(_motor_id, ODriveAxisState::IDLE);
    }

public:
    MotorController(std::shared_ptr<ODriveSocket> odrv, int motor_id)
        : Estop(), _odrv_socket(odrv), _motor_id(motor_id) { }

    ~MotorController() { 
        _odrv_socket->setAxisState(_motor_id, ODriveAxisState::IDLE);
    }

    void set_axis_state(const ODriveAxisState axis_state) {
        _odrv_socket->setAxisState(_motor_id, axis_state);
    }

    void set_control_mode(const ODriveControlMode control_mode, const ODriveInputMode input_mode = ODriveInputMode::PASSTHROUGH) {
        _odrv_socket->setControlMode(_motor_id, control_mode, input_mode);
    }

    void set_torque(const float torque_setpoint) {
        _odrv_socket->setTorque(_motor_id, torque_setpoint);
    }

    float get_position() {
        return _odrv_socket->getPositionEstimate(_motor_id);
    }

    float get_velocity() {
        return _odrv_socket->getVelocityEstimate(_motor_id);
    }

    float get_torque_estimate() {
        float iq = _odrv_socket->getIqMeasured(_motor_id);
        return _torque_constant * iq;
    }

    float get_current_setpoint() {
        return _odrv_socket->getIqSetpoint(_motor_id);
    }

    float get_current_measured() {
        return _odrv_socket->getIqMeasured(_motor_id);
    }

};