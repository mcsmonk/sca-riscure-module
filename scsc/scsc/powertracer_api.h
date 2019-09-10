#ifndef _POWER_TRACER_API_H
#define _POWER_TRACER_API_H

#include <windows.h>

   #ifdef __cplusplus
      #define POWERTRACER_API extern "C" __declspec( dllexport )
   #else
      #define POWERTRACER_API __declspec( dllexport )
   #endif

typedef PVOID  pt_handle;
typedef struct pt_device pt_device;
typedef pt_device * PPTDEVICE;
typedef enum   pt_tx_error_handling pt_tx_error_handling;
typedef struct pt_version pt_version;
typedef enum   pt_pcb_version pt_pcb_version;
typedef enum   PT_STATUS PT_STATUS;
typedef enum   pt_com_channel pt_com_channel;
typedef enum   pt_rs232_baudrate pt_rs232_baudrate;
typedef enum   pt_rs232_parity pt_rs232_parity;
typedef enum   pt_rs232_stop pt_rs232_stop;

/**
 * @ingroup misc
 * @brief Enumeration type of API function return status.
 *
 * Only critical failures are enumerated into separate items, other minor issues are enumerated by PT_FAILED.
 */
enum PT_STATUS {
     PT_OK                       = 0,   /*!< Operation completed succesfully. */
     PT_NOT_FOUND                = 1,   /*!< Power Tracer could not be found. */
     PT_NOT_COMPATIBLE           = 2,   /*!< Function not compatible with this version Power Tracer. */
     PT_VOLTAGE_OUT_RANGE        = 3,   /*!< The voltage setting for the potmeter is out of range. */
     PT_OFFSET_OUT_RANGE         = 4,   /*!< The offset setting for the potmeter is out of range. */
     PT_GAIN_OUT_RANGE           = 5,   /*!< The gain setting for the potmeter is out of range. */
     PT_CAPACITOR_LOW_VOLTAGE    = 6,   /*!< The capacitors are not charged sufficiently. */
     PT_IO_ERROR                 = 7,   /*!< USB Communication error. */
     PT_INVALID_HANDLE           = 8,   /*!< An invalid Power Tracer handle was specified. */
     PT_INVALID_COM_CHANNEL      = 9,   /*!< An invalid communication channel was selected. */
     PT_INVALID_BAUDRATE         = 10,  /*!< An invalid RS232 baudrate was specified. */
     PT_INVALID_PARITY           = 11,  /*!< An invalid RS232 parity mode was specified. */
     PT_INVALID_STOP             = 12,  /*!< An invalid RS232 stop bit length was specified. */
     PT_FAILED                   = 13,  /*!< The operation failed (reason unknown). */
     PT_ALREADY_OPENED           = 14,  /*!< The Power Tracer has already been opened. */
     PT_NOT_OPENED               = 15,  /*!< The Power Tracer has not been opened. */
     PT_LENGTH_OUT_RANGE         = 16,  /*!< An invalid length was specified. */
     PT_TRIGGER_NOT_READY        = 17,  /*!< The trigger is not yet ready to be armed. */
};

/**
 * @ingroup sc_com
 * @brief Enumeration type for the smart card transmitter error handling.
 */
enum pt_tx_error_handling {
   CONTINUE = 0,                        /*!< Continue the transmission when a parity error has been detected, but the index of the first error charater will be returned (ZERO indexed). */
   HALT     = 1,                        /*!< Halt the transmission if a parity error has been detected. The left-over data will be discarded and the index of the first error character will be returned (ZERO indexed). */
};

/**
 * @ingroup misc
 * @brief Enumeration type for the Power Tracer PCB version.
 */
enum pt_pcb_version {
   PT_PCB_VERSION_4B = 0x31,            /*!< value assigned to PCB version 4B. */
};

/**
 * @ingroup sc_com
 * @brief Enumeration type for the Power Tracer communication channels.
 */
enum pt_com_channel {
   SMARTCARD    = 0x1,                  /*!< Select smart card interface as primary communication channel. */
   EXT_SERIAL   = 0x2,                  /*!< Select external serial interface as primary communication channel. */
};

/**
 * @ingroup sc_com
 * @brief Enumeration type for supported RS232 baudrates.
 */
enum pt_rs232_baudrate {
   BAUD9600   = 0,                      /*!< 9600 bps baudrate. */
   BAUD19200  = 1,                      /*!< 19200 bps baudrate. */
   BAUD38400  = 2,                      /*!< 38400 bps baudrate. */
   BAUD57600  = 3,                      /*!< 57600 bps baudrate. */
   BAUD115200 = 4,                      /*!< 115200 bps baudrate. */
};

/**
 * @ingroup sc_com
 * @brief Enumeration type for supported RS232 parity modes.
 */
enum pt_rs232_parity {
   NONE = 0,                           /*!< Generate/Expect NO parity in the serial transmitter/receiver. */
   ODD  = 1,                           /*!< Generate/Expect ODD parity in the serial transmitter/receiver. */
   EVEN = 2,                           /*!< Generate/Expect EVEN parity in the serial transmitter/receiver. */
};

/**
 * @ingroup sc_com
 * @brief Enumeration type for supported RS232 stop bit length.
 */
enum pt_rs232_stop {
   ONE_STOP = 0,                       /*!< Generate/Expect ONE stop bit before transmission of another character can be started in the serial transmitter/receiver. */
   TWO_STOP = 1,                       /*!< Generate/Expect TWO stop bit before transmission of another character can be started in the serial transmitter/receiver. */
};

/**
 * \ingroup misc
 * @brief Data structure for the Power Tracer version information.
 */
struct pt_version {
   pt_pcb_version pcb_version;          /*!< PCB version of Power Tracer. */
   unsigned char bitstream_version;     /*!< Bitstream version of Power Tracer. */
   unsigned char firmware_version;      /*!< Firmware version of Power Tracer. */
};

/**
 * \ingroup init
 * @brief Data structure for the Power Tracer device information.
 */
struct pt_device {
   DWORD          locationId;                     /*!< Location ID of the Power Tracer device (Obsolete). */
   char           serialNumber[16];               /*!< Serial number of the Power Tracer device. */
   char           description[64];                /*!< Description of the Power Tracer device. */
   pt_handle      handle;                         /*!< Handle of the Power Tracer device. */
   pt_version     version;                        /*!< Version information. */
   void           *internal_config;               /*!< Structure for internal use. */
};

/**
 * \ingroup sc_com
 * @brief Set the read timeout to the USB communication.
 *
 * <b>NOTE:</b> This parameter is also used as read timeout of pt_read() function.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.\n
 * @param timeout The timeout length in miliseconds.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_write_timeout()
 */
POWERTRACER_API PT_STATUS pt_set_read_timeout(pt_device * powertracer_device, unsigned int timeout);

/**
 * \ingroup sc_com
 * @brief Set the write timeout to the USB communication.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param timeout The timeout length in miliseconds.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_read_timeout()
 */
POWERTRACER_API PT_STATUS pt_set_write_timeout(pt_device * powertracer_device, unsigned int timeout);

/**
 * \ingroup init
 * @brief Obtain the number of Power Tracer devices ready for use.
 * @param count A pointer to an unsigned integer that stores the detected device count.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_device_get_info()
 */
POWERTRACER_API PT_STATUS pt_device_list(unsigned int * count);

/**
 * \ingroup init
 * @brief Obtain the device information of the specified Power Tracer device and store it to a powertracer data structure.
 * 
 * <b>NOTE:</b> The pt_device_get_info() should be preceeded by the pt_device_list() call.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param index The index to the Power Tracer device from which the device information is collected.
 * <b>NOTE:</b> Assigning ZERO to this parameter will indicate getting the information from the first detected device.\n
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_device_list()
 * @see pt_open()
 */
POWERTRACER_API PT_STATUS pt_device_get_info(pt_device * powertracer_device, unsigned int index);

/**
 * \ingroup misc
 * @brief Obtain the device version, and print it into a byte array in the format of XX.YY.ZZ, where X stands for PCB version, Y for bitstream version and Z for firmware version.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param version_buffer Pointer to the byte array where the version information is stored.
 * @param buffer_size The length of the byte array, which should be at least 9 bytes large.(i.e. XX.YY.ZZ + '\0')
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 */
POWERTRACER_API PT_STATUS pt_get_version(pt_device * powertracer_device, unsigned char * version_buffer, unsigned int buffer_size);

/**
 * \ingroup init
 * @brief Open the Power Tracer device described by pt_device structure.
 *
 * <b>NOTE:</b> The open call has to be preceeded by a pt_device_get_info() call which collects information neccessary for the opening operation.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_device_get_info
 */
POWERTRACER_API PT_STATUS pt_open(pt_device * powertracer_device);

/**
 * \ingroup init
 * @brief Close the Power Tracer device described by pt_device structure.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_open()
 */
POWERTRACER_API PT_STATUS pt_close(pt_device * powertracer_device);

/**
 * \ingroup sc_com
 * @brief Select the communication channel, through which data bytes are sent and received.
 *
 * <b>NOTE:</b> When the Power Tracer is powered up, the default communication channel is SMARTCARD.\n
 * <b>NOTE:</b> When a communication channel is de-selected, its transceivers are put into reset and unfetched data is lost.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param channel_sel The new communication channel to be selected and used. Possible values are:<br>
 * <b>SMARTCARD</b>, if data bytes are to be sent through the Power Tracer smart card serial interface,<br>
 * <b>EXT_SERIAL</b>, if data bytes are to be sent through the Power Tracer RS232 serial interface.<br>
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_INVALID_COM_CHANNEL if an invalid communication channel was given to the function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_get_communication_channel()
 */
POWERTRACER_API PT_STATUS pt_set_communication_channel(pt_device * powertracer_device, pt_com_channel channel_sel);

/**
 * \ingroup sc_ctrl
 * @brief Set the voltage supplied on the VCC pin of smart card interface.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param voltage The voltage value to be configured. Ranging from 1.8 volt to 6.0 volt, with resolution of 0.1 volt. When Power Tracer is powered up, the default voltage is 1.8 volt.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STAUTS.PT_VOLTAGE_OUT_RANGE if the voltage setpoint is out of the range.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_get_smartcard_voltage()
 * @see pt_get_smartcard_voltage_boundary()
 * @see pt_get_smartcard_voltage_stepsize()
 */
POWERTRACER_API PT_STATUS pt_set_smartcard_voltage(pt_device * powertracer_device, double voltage);

/**
 * \ingroup sig
 * @brief Set the offset of the Power Tracer Signal port.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param offset The desired offset value to be configured. Ranging from -2.0 volt to 2.0 volt, with resolution of 0.1 volt. When Power Tracer is powered up, the default offset is 0.0 volt.
 * @warning Not thread safe !
 * @deprecated Replacement function pt_set_offset_by_current()
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STAUTS.PT_OFFSET_OUT_RANGE if the offset to be configured is out of the range.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_get_offset()
 * @see pt_get_offset_boundary()
 * @see pt_get_offset_stepsize()
 */
POWERTRACER_API PT_STATUS pt_set_offset(pt_device * powertracer_device, double offset);

/**
 * \ingroup sig
 * @brief Set the offset current to the Power Tracer Signal port.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param offset_current The offset current(mA) for the Power Tracer Signal port, ranging from -30 to 0.
 * @warning Not thread safe !
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STAUTS.PT_OFFSET_OUT_RANGE if the offset to be configured is out of the range.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_get_offset_by_current()
 * @see pt_get_offset_boundary_by_current()
 * @see pt_get_offset_stepsize_by_current()
 */
POWERTRACER_API PT_STATUS pt_set_offset_by_current(pt_device * powertracer_device, int offset_current);

/**
 * \ingroup sig
 * @brief Set the gain of the Power Tracer "signal" port.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param gain The desired gain value to be configured. The gain is expressed by an integer ranging from 100 ~ 200 to represent 100% ~ 200%, with resolution of 1%. When Power Tracer is powered up, the default gain value is 100.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STAUTS.PT_GAIN_OUT_RANGE if the gain to be configured is out of the range.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_get_gain()
 * @see pt_get_gain_boundary()
 * @see pt_get_gain_stepsize()
 */
POWERTRACER_API PT_STATUS pt_set_gain(pt_device * powertracer_device, unsigned int gain);

/**
 * \ingroup sc_com
 * @brief Write bytes over the selected communication channel.
 *
 * <b>NOTE:</b> The function call will be blocked until the actual transmission is complete.\n
 * <b>NOTE:</b> Data bytes sent by this function are directed to the communication channel set by pt_set_communication_channel().\n
 * <b>NOTE:</b> Upon finishing sending data bytes, the trigger status of Power Tracer is forced to 'disarmed'.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param command A pointer to a byte array containing the bytes to be transmitted.
 * @param command_length The total length of the command frame in byte. The length should not exceed 1024 bytes.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_LENGTH_OUT_RANGE if an invalid command length was specified.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_read()
 * @see pt_set_communication_channel()
 */
POWERTRACER_API PT_STATUS pt_write(pt_device * powertracer_device, unsigned char * command, unsigned int command_length);

/**
 * \ingroup sc_com
 * @brief Read bytes over the selected communication channel.
 *
 * <b>NOTE:</b> The read timeout specified pt_set_read_timeout() also applies to this function.\n
 * <b>NOTE:</b> The function will try to read the indicated number of bytes from the communication channel set by pt_set_communication_channel().\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param response A pointer to a byte array buffer where fetched data bytes are stored.
 * @param read_length The desired length of bytes to read into the byte array. The length should not exceed 1024. <b>NOTE:</b> Assigning ZERO to this parameter cause the function to return any bytes (no more than 1024 bytes) that are currently available from the selected communication channel.
 * @param actual_read_length The number of bytes being actually fetched out.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_LENGTH_OUT_RANGE if the read length is out of range.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_write()
 * @see pt_set_communication_channel()
 */
POWERTRACER_API PT_STATUS pt_read(pt_device * powertracer_device, unsigned char * response, unsigned int read_length, unsigned int * actual_read_length);

/**
 * \ingroup sc_ctrl
 * @brief Configure the clock frequency(MHz) supplied on the smart card slot CLK pin.
 *
 * <b>NOTE:</b> The recommended frequency resolution is 0.05MHz, however, frequency setpoints with finer resoltions could also be achieved but without any guarantee.\n
 * <b>NOTE:</b> This function does not affect EXT_SERIAL communication channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param desired_frequency The desired smart card clock frequency ranging from 1.0 to 10.0 in MHz. When Power Tracer is powered, the default smart card CLK frequency is 1MHz.
 * @param actual_frequency The actualy smart card clock frequency achieved, also in MHz unit.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_get_smartcard_frequency()
 * @see pt_get_smartcard_frequency_boundary()
 * @see pt_get_smartcard_frequency_stepsize()
 */
POWERTRACER_API PT_STATUS pt_set_smartcard_frequency(pt_device * powertracer_device, double desired_frequency, double * actual_frequency);

/**
 * \ingroup sc_com
 * @brief Configure the clock rate conversion integer (F) and baudrate adjustment integer (D) required for smart card communication.
 *
 * <b>NOTE:</b> The default F=372 and D=1 when the Power Tracer is powered up.\n
 * <b>NOTE:</b> The rounded up quotient of F divided by D should be equal or larger than 5 and should not exceed 4095.\n
 * <b>NOTE:</b> This function does not affect EXT_SERIAL communication channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param f The desired clock rate conversion interger value.
 * @param d The desired baudrate adjustment integer value.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_get_etu()
 */
POWERTRACER_API PT_STATUS pt_set_f_d(pt_device * powertracer_device, unsigned int f, unsigned int d);

/**
 * \ingroup sc_com
 * @brief Configure the character guard time required for smart card communication.
 *
 * <b>NOTE:</b> The default CGT=12 when the Power Tracer is powered up.\n
 * <b>NOTE:</b> This function does not affect EXT_SERIAL communication channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param cgt_value The character guard time (CGT) to be configured, ranging from 11 to 511.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_get_cgt()
 */
POWERTRACER_API PT_STATUS pt_set_cgt(pt_device * powertracer_device, unsigned int cgt_value);

/**
 * \ingroup sc_com
 * @brief Enable/Disable the smart card transmitter from checking an ACK/NACK from the smart card during a character frame.
 *
 * <b>NOTE:</b> When the parity ACK/NACK check is disabled, the smart card transmitter will assume all characters are correctly received by the smart card.\n
 * <b>NOTE:</b> To support character retransmission with T=0 card, the transmitter error handling has to be set to 'HALT' mode with pt_set_tx_error_handling().\n
 * <b>NOTE:</b> This function does not affect EXT_SERIAL communication channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param enabled A boolean variable serving as a control switch. TRUE/FALSE for on/off.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_is_tx_error_checking_enabled()
 * @see pt_set_tx_error_handling()
 */
POWERTRACER_API PT_STATUS pt_enable_tx_error_checking(pt_device * powertracer_device, BOOL enabled);

/**
 * \ingroup sc_com
 * @brief Enable/Disable the smart card receiver from signaling an ACK/NACK to the smart card during a character frame.
 *
 * <b>NOTE:</b> This configuration only affects the SMARTCARD communication channel.\n
 * <b>NOTE:</b> When the parity ACK/NACK signaling is disabled, the Power Tracer smart card receiver will not inspect the parity correctness of a received character and always ACK on character.\n
 * <b>NOTE:</b> This function does not affect EXT_SERIAL communication channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param enabled A boolean variable serving as a control switch. TRUE/FALSE for on/off.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_is_rx_error_signaling_enabled()
 */
POWERTRACER_API PT_STATUS pt_enable_rx_error_signaling (pt_device * powertracer_device, BOOL enabled);

/**
 * \ingroup sc_com
 * @brief Configure the behavior of the transmitter when an parity error has been detected.
 *
 * <b>NOTE:</b> This configuration only affects the SMARTCARD communication channel.\n
 * <b>NOTE:</b> The transmitter error checking has to be enabled, with pt_enable_tx_error_checking(), in order to support 'HALT' error handling.\n
 * <b>NOTE:</b> This function does not affect EXT_SERIAL communication channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param error_handling The desired error handling behaviour. Possible values are COUNTINUE and HALT.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_enable_tx_error_checking()
 */
POWERTRACER_API PT_STATUS pt_set_tx_error_handling(pt_device * powertracer_device, pt_tx_error_handling error_handling);

/**
 * \ingroup sc_com
 * @brief Check if any smart card transmission error has occured. Report the index of the error byte if an error has been detected.
 *
 * <b>NOTE:</b> It is recommended to configure the transmitter error handling to HALT if the detection of transmission error is critical. Since only the latest transmssion error is reported.\n
 * <b>NOTE:</b> The regisers storing error infomation will be cleared to 'Error never happend' status after the function call.\n
 * <b>NOTE:</b> This function only reports Tx error status of SMARTCARD channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param error_occured A pointer to a boolean variable indicating if any error has occured.
 * @param error_index A pointer to an unsigned integer that stores the index to the error character. (0xffff if no error occurred)
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_enable_tx_error_checking()
 */
POWERTRACER_API PT_STATUS pt_get_tx_error_status (pt_device * powertracer_device, BOOL * error_occured, unsigned int * error_index);

/**
 * \ingroup sc_com
 * @brief Check if any smart card reception error has occured.
 *
 * <b>NOTE:</b> The regisers storing error infomation will be cleared to 'Error never happend' status after the function call.\n
 * <b>NOTE:</b> This function only reports Rx error status of SMARTCARD channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param error_occured A boolean variable indicating if any error has occured.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_enable_rx_error_signaling ()
 */
POWERTRACER_API PT_STATUS pt_get_rx_error_status (pt_device * powertracer_device, BOOL * error_occured);

/**
 * \ingroup sc_ctrl
 * @brief Perform a warm reset to the smart card.
 *
 * <b>NOTE:</b> Warm reset will not change the ETU (F and D) setting, nor the parity ACK/NACK checking and signaling status. Restoring parameters to desired condition is up to the user.\n
 * <b>NOTE:</b> This function does not affect EXT_SERIAL communication channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_smartcard_cold_reset()
 */
POWERTRACER_API PT_STATUS pt_smartcard_warm_reset(pt_device * powertracer_device);

/**
 * \ingroup sc_ctrl
 * @brief Perform a cold reset to the smart card.
 *
 * <b>NOTE:</b> Cold reset will not change the ETU (F and D) setting, nor the parity ACK/NACK checking and signaling status. Restoring parameter to desired condition is up to the user.\n
 * <b>NOTE:</b> This function does not affect EXT_SERIAL communication channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_smartcard_warm_reset()
 */
POWERTRACER_API PT_STATUS pt_smartcard_cold_reset(pt_device * powertracer_device);

/**
 * \ingroup trg
 * @brief Arm/Disarm the Power Tracer so that it could generate a series of events such as signaling a trigger, powering the card with capacitor(SMARTCARD channel only) and temporarily cutting the card clock supply(SMARTCARD channel only) immediatly after the transmission of the last byte with an optional delay.
 *
 * <b>NOTE:</b> To manipulate the timing of the triggering, capacitor powering and clock gating, see pt_set_trigger_delay(), pt_set_dcdc_off_delay(), pt_set_dcdc_off_duration(), pt_set_clock_off_delay(), and pt_set_clock_off_duration() functions.\n
 * <b>NOTE:</b> This function affects the trigger behavior under both SMARTCARD channel and EXT_SERIAL channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param arm if 'TRUE' then the function will try to arm the trigger, disarm it otherwise.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_CAPACITOR_LOW_VOLTAGE if the internal capacitors are not sufficiently charged.<br>
 * PT_TRIGGER_NOT_READY if the trigger is still busy and not ready for the next operation.
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_is_armed()
 */
POWERTRACER_API PT_STATUS pt_set_armed(pt_device * powertracer_device, BOOL arm);

/**
 * \ingroup trg
 * @brief Set the trigger delay.
 *
 * <b>NOTE:</b> The delay referred here is relative to the end of the last byte transmission.\n
 * <b>NOTE:</b> This function affects the trigger behavior under both SMARTCARD channel and EXT_SERIAL channel\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param delay The delay in microsecond. It should not exceed 4,995,904 microseconds. <b>NOTE:</b> Assigning 0 to this parameter to indicate that no delay should apply on the generation of this event.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_LENGTH_OUT_RANGE if the specified \b delay length is out of valid range.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_get_trigger_delay()
 */
POWERTRACER_API PT_STATUS pt_set_trigger_delay(pt_device * powertracer_device, unsigned int delay);

/**
 * \ingroup trg
 * @brief Set the delay before the capacitor array are turned on and supply power to the smart card.
 *
 * <b>NOTE:</b> The delay referred here is relative to the start of transmission of the last command byte.\n
 * <b>NOTE:</b> This function does not affect EXT_SERIAL communication channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param delay The delay in microsecond. It should not exceed 4,995,904 microseconds. <b>NOTE:</b> Assigning 0 to this parameter to indicate that no delay should apply on the generation of this event.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_LENGTH_OUT_RANGE if the specified \b delay length is out of valid range.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_get_dcdc_off_delay()
 * @see pt_set_dcdc_off_duration()
 * @see pt_get_dcdc_off_duration()
 */
POWERTRACER_API PT_STATUS pt_set_dcdc_off_delay(pt_device * powertracer_device, unsigned int delay);

/**
 * \ingroup trg
 * @brief Set the duration of using the capacitor array for the smart card power supply.
 *
 * <b>NOTE:</b> The duration will apply after the delay setting to this event expires.\n
 * <b>NOTE:</b> This function does not affect EXT_SERIAL communication channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param duration The duration in microsecond. It should not exceed 5,000,000 microseconds. <b>NOTE:</b> Assigning 0 to this parameter to indicate that this event should not be generated and ignore its delay setting.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_LENGTH_OUT_RANGE if the specified \b duration length is out of valid range.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_get_dcdc_off_duration()
 * @see pt_set_dcdc_off_delay()
 * @see pt_get_dcdc_off_delay()
 */
POWERTRACER_API PT_STATUS pt_set_dcdc_off_duration(pt_device * powertracer_device, unsigned int duration);

/**
 * \ingroup trg
 * @brief Set the delay before turning of the smart card clock supply.
 *
 * <b>NOTE:</b> The delay referred here is relative to the start of transmission of the last command byte.\n
 * <b>NOTE:</b> This function does not affect EXT_SERIAL communication channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param delay delay The delay in microsecond. It should not exceed 4,995,904 microseconds. <b>NOTE:</b> Assigning 0 to this parameter to indicate that no delay should apply on the generation of this event.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_LENGTH_OUT_RANGE if the specified \b delay length is out of valid range.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_get_clock_off_delay()
 * @see pt_set_clock_off_duration()
 * @see pt_get_clock_off_duration()
 */
POWERTRACER_API PT_STATUS pt_set_clock_off_delay(pt_device * powertracer_device, unsigned int delay);

/**
 * \ingroup trg
 * @brief Set the duration of turning off the smart card clock supply.
 *
 * <b>NOTE:</b> The duration will apply after the delay setting to this event expires.\n
 * <b>NOTE:</b> This function does not affect EXT_SERIAL communication channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param duration The duration in microsecond. Duration = 0 indicates the clock should not be turned off and delay value will be ignored.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_LENGTH_OUT_RANGE if the specified \b duration length is out of valid range.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_get_clock_off_duration()
 * @see pt_set_clock_off_delay()
 * @see pt_get_clock_off_duration()
 */
POWERTRACER_API PT_STATUS pt_set_clock_off_duration(pt_device * powertracer_device, unsigned int duration);

/**
 * \ingroup sc_ctrl
 * @brief Power up the Power Tracer smart card slot.
 *
 * <b>NOTE:</b> The voltage applied on the VCC line of smart card slot depends on the value set by pt_set_smartcard_voltage().\n
 * <b>NOTE:</b> The smart card interface is in power-down status when Power Tracer is powered.\n
 * <b>NOTE:</b> This function does not affect EXT_SERIAL communication channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_smartcard_powerdown()
 * @see pt_is_smartcard_powered()
 */
POWERTRACER_API PT_STATUS pt_smartcard_powerup(pt_device * powertracer_device);

/**
 * \ingroup sc_ctrl
 * @brief Power down the Power Tracer smart card slot.
 *
 * <b>NOTE:</b> Powering down the smartcard will not change the ETU (F and D) setting, nor the partity ACK/NACK checking and signaling status.\n
 * <b>NOTE:</b> This function does not affect EXT_SERIAL communication channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_smartcard_powerup()
 * @see pt_is_smartcard_powered()
 */
POWERTRACER_API PT_STATUS pt_smartcard_powerdown(pt_device * powertracer_device);

/**
 * \ingroup sc_com
 * @brief Get the currently selected communication channel.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param com_channel A pointer to pt_com_channel enumeration type bearing the returned selected channel. Possible returned values are:<br>
 * <b>SMARTCARD</b>, if data bytes are to be sent through the Power Tracer smart card serial interface,<br>
 * <b>EXT_SERIAL</b>, if data bytes are to be sent through the Power Tracer RS232 serial interface.<br>
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_communication_channel()
 */
POWERTRACER_API PT_STATUS pt_get_communication_channel(pt_device * powertracer_device, pt_com_channel * com_channel);

/**
 * \ingroup sc_ctrl
 * @brief Get the voltage that will be applied on smart card when it is powered.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param voltage A pointer to a double variable that stores the obtained voltage value.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_smartcard_voltage()
 * @see pt_get_smartcard_voltage_boundary()
 * @see pt_get_smartcard_voltage_stepsize()
 */
POWERTRACER_API PT_STATUS pt_get_smartcard_voltage(pt_device * powertracer_device, double * voltage);

/**
 * \ingroup sig
 * @brief Get the offset voltage applied on the Power Tracer Signal port.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param offset A pointer to a double variable that stores the obtained offset value.
 * @warning Not thread safe !
 * @deprecated Replacement function pt_get_offset_by_current()
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_offset()
 * @see pt_get_offset_boundary()
 * @see pt_get_offset_stepsize()
 */
POWERTRACER_API PT_STATUS pt_get_offset(pt_device * powertracer_device, double * offset);

/**
 * \ingroup sig
 * @brief Get the offset current applied on the Power Tracer Signal port.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param offset_current A pointer to a integer variable that stores the obtained offset current value.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STAUTS.PT_OFFSET_OUT_RANGE if the offset current cannot be determined.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_offset_by_current()
 * @see pt_get_offset_boundary_by_current()
 * @see pt_get_offset_stepsize_by_current()
 */
POWERTRACER_API PT_STATUS pt_get_offset_by_current(pt_device * powertracer_device, int * offset_current);

/**
 * \ingroup sig
 * @brief Get the gain applied on the Power Tracer 'signal' port.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param gain A pointer to a double variable that stores the obtained gain value.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_gain()
 * @see pt_get_gain_boundary()
 * @see pt_get_gain_stepsize()
 */
POWERTRACER_API PT_STATUS pt_get_gain(pt_device * powertracer_device, unsigned int * gain);

/**
 * \ingroup sc_ctrl
 * @brief Get maximum and minimum supported voltage of smart card power.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param min A pointer to a double variable bearing the returned minimum voltage allowed.
 * @param max A pointer to a double variable bearing the returned maximum voltage allowed.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_smartcard_voltage()
 * @see pt_get_smartcard_voltage()
 * @see pt_get_smartcard_voltage_stepsize()
 */
POWERTRACER_API PT_STATUS pt_get_smartcard_voltage_boundary(pt_device * powertracer_device, double * min, double * max);

/**
 * \ingroup sig
 * @brief Get maximum and minimum supported offset voltage.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param min A pointer to a double variable bearing the returned minimum offset allowed.
 * @param max A pointer to a double variable bearing the returned maximum offset allowed.
 * @warning Not thread safe !
 * @deprecated Replacement function pt_get_offset_boundary_by_current()
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_offset()
 * @see pt_get_offset()
 * @see pt_get_offset_stepsize()
 */
POWERTRACER_API PT_STATUS pt_get_offset_boundary(pt_device * powertracer_device, double * min, double * max);

/**
 * \ingroup sig
 * @brief Get maximum and minimum supported offset current.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param min A pointer to a double variable bearing the returned minimum offset current allowed.
 * @param max A pointer to a double variable bearing the returned maximum offset current allowed.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_offset_by_current()
 * @see pt_get_offset_by_current()
 * @see pt_get_offset_stepsize_by_current()
 */
POWERTRACER_API PT_STATUS pt_get_offset_boundary_by_current(pt_device * powertracer_device, int * min, int * max);

/**
 * \ingroup sig
 * @brief Get maximum and minimum supported gain percentage.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param min A pointer to an unsigned integer variable bearing the returned minimum gain allowed.
 * @param max A pointer to an unsigned integer variable bearing the returned maximum gain allowed.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_gain()
 * @see pt_get_gain()
 * @see pt_get_gain_stepsize()
 */
POWERTRACER_API PT_STATUS pt_get_gain_boundary(pt_device * powertracer_device, unsigned int * min, unsigned int * max);

/**
 * \ingroup sc_ctrl
 * @brief Get the resolution of the smart card voltage setting.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param step_size A pointer to a double variable bearing the returned voltage resolution.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_smartcard_voltage()
 * @see pt_get_smartcard_voltage()
 * @see pt_get_smartcard_voltage_boundary()
 */
POWERTRACER_API PT_STATUS pt_get_smartcard_voltage_stepsize(pt_device * powertracer_device, double * step_size);

/**
 * \ingroup sig
 * @brief Get the resolution of the Power Tracer 'signal' port offset setting.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param step_size A pointer to a double variable bearing the returned offset resolution.
 * @warning Not thread safe !
 * @deprecated Replacement function pt_get_offset_stepsize_by_current()
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_offset()
 * @see pt_get_offset()
 * @see pt_get_offset_boundary()
 */
POWERTRACER_API PT_STATUS pt_get_offset_stepsize(pt_device * powertracer_device, double * step_size);

/**
 * \ingroup sig
 * @brief Get the offset current setpoint resolution.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param step_size A pointer to a double variable bearing the returned offset resolution.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_offset_by_current()
 * @see pt_get_offset_by_current()
 * @see pt_get_offset_boundary_by_current()
 */
POWERTRACER_API PT_STATUS pt_get_offset_stepsize_by_current(pt_device * powertracer_device, int * step_size);

/**
 * \ingroup sig
 * @brief Get the resolution of the Power Tracer 'signal' port gain setting.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param step_size A pointer to an unsigned integer variable bearing the returned gain resolution.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_gain()
 * @see pt_get_gain()
 * @see pt_get_gain_boundary()
 */
POWERTRACER_API PT_STATUS pt_get_gain_stepsize(pt_device * powertracer_device, unsigned int * step_size);

/**
 * \ingroup trg
 * @brief Get the trigger delay setting.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param delay A pointer to an unsigned integer that stores the obtained delay value.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_trigger_delay()
 */
POWERTRACER_API PT_STATUS pt_get_trigger_delay(pt_device * powertracer_device, unsigned int * delay);

/**
 * \ingroup trg
 * @brief Check if the Power Tracer is armed to generate timed events(i.e. triggering, switching to capacitor power supply and clock gating).
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param armed A pointer to a boolean variable that stores the obtained inquiry result.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_armed()
 */
POWERTRACER_API PT_STATUS pt_is_armed(pt_device * powertracer_device, BOOL * armed);

/**
 * \ingroup trg
 * @brief Get the delay before the capacitor array is switched on for the smart card power supply.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param delay A pointer to an unsigned integer that stores the obtained delay value.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_dcdc_off_delay()
 * @see pt_set_dcdc_off_duration()
 * @see pt_get_dcdc_off_duration()
 */
POWERTRACER_API PT_STATUS pt_get_dcdc_off_delay(pt_device * powertracer_device, unsigned int * delay);

/**
 * \ingroup trg
 * @brief Get the duration of using the capacitor array as the smart card power supply.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param duration A pointer to an unsigned integer that stores the obtained duration value.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_dcdc_off_duration()
 * @see pt_set_dcdc_off_delay()
 * @see pt_get_dcdc_off_delay()
 */
POWERTRACER_API PT_STATUS pt_get_dcdc_off_duration(pt_device * powertracer_device, unsigned int * duration);

/**
 * \ingroup trg
 * @brief Get the delay before the smart card clock supply is switched off.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param delay A pointer to an unsigned integer that stores the obtained delay value.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_clock_off_delay()
 * @see pt_set_clock_off_duration()
 * @see pt_get_clock_off_duration()
 */
POWERTRACER_API PT_STATUS pt_get_clock_off_delay(pt_device * powertracer_device, unsigned int * delay);

/**
 * \ingroup trg
 * @brief Get the duration of the smart card clock supply being switched off.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param duration A pointer to an unsigned integer that stores the obtained duration value.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_clock_off_duration()
 * @see pt_set_clock_off_delay()
 * @see pt_get_clock_off_delay()
 */
POWERTRACER_API PT_STATUS pt_get_clock_off_duration(pt_device * powertracer_device, unsigned int * duration);

/**
 * \ingroup sc_com
 * @brief Get the elementary time unit (ETU) setting used to setup the communication between Power Tracer and smart card.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param etu A pointer to an unsigned integer that stores the obtained ETU setting.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_f_d()
 */
POWERTRACER_API PT_STATUS pt_get_etu(pt_device * powertracer_device, unsigned int * etu);

/**
 * \ingroup sc_com
 * @brief Get the character guard time (CGT) setting used to setup the communication between Power Tracer and smart card.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param cgt A pointer to an unsigned integer that stores the obtained CGT setting.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_cgt()
 */
POWERTRACER_API PT_STATUS pt_get_cgt(pt_device * powertracer_device, unsigned int * cgt);

/**
 * \ingroup sc_ctrl
 * @brief Get the smart card clock frequency (MHz) used by the Power Tracer.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param frequency A pointer to a double variable that stores the obtained frequency value.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_smartcard_frequency()
 */
POWERTRACER_API PT_STATUS pt_get_smartcard_frequency(pt_device * powertracer_device, double * frequency);

/**
 * \ingroup sc_com
 * @brief Check if the transmitter will look at the ACK/NACK bit to determine the correctness of transmission.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param enabled A pointer to a boolean variable that stores the obtained inquiry result.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_enable_tx_error_checking()
 */
POWERTRACER_API PT_STATUS pt_is_tx_error_checking_enabled(pt_device * powertracer_device, BOOL * enabled);

/**
 * \ingroup sc_com
 * @brief Check if the receiver will signal the smart card a NACK bit when parity error is detected.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param enabled A pointer to a boolean variable that stores the obtained inquiry result.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_enable_rx_error_signaling ()
 */
POWERTRACER_API PT_STATUS pt_is_rx_error_signaling_enabled(pt_device * powertracer_device, BOOL * enabled);

/**
 * \ingroup sc_com
 * @brief Check if the smart card is powered by the Power Tracer.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param powered A pointer to a boolean variable that stores the obtained inquiry result.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_smartcard_powerup()
 * @see pt_smartcard_powerdown()
 */
POWERTRACER_API PT_STATUS pt_is_smartcard_powered(pt_device * powertracer_device, BOOL * powered);

/**
 * \ingroup misc
 * @brief Check if the smart card is inserted.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param inserted A pointer to a boolean variable that indicates whether the card is inserted.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 */
POWERTRACER_API PT_STATUS pt_is_smartcard_inserted(pt_device * powertracer_device, BOOL * inserted);

/**
 * \ingroup sc_com
 * @brief Configure the paramters of EXT_SERIAL channel
 *
 * <b>NOTE:</b> This function does not affect SMARTCARD communication channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param baudrate Enumeration type defining a set of supported baudrates. Possible values are:<br>
 * <b>BAUD9600</b>, for 9600 bps baudrate,<br>
 * <b>BAUD19200</b>, for 19200 bps baudrate,<br>
 * <b>BAUD38400</b>, for 38400 bps baudrate,<br>
 * <b>BAUD57600</b>, for 57600 bps baudrate,<br>
 * <b>BAUD115200</b>, for 115200 bps baudrate,<br>
 * @param parity Enumeration type defining supported parity mode. Possible values are:<br>
 * <b>NONE</b>, if parity bit will not be included,<br>
 * <b>ODD</b>, if the odd-parity bit will be included,<br>
 * <b>EVEN</b>, if the even-parity bit will be included.<br>
 * @param stop Enumeration type defining supported stop bit length. Possible values are:<br>
 * <b>ONE_STOP</b>, if one stop bit is attached to the end of a character frame,<br>
 * <b>TWO_STOP</b>, if two stop bits are attached to the end of a character frame.<br>
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_INVALID_BAUDRATE if an invalid baudrate was chosen.<br>
 * PT_STATUS.PT_INVALID_PARITY if an invalid parity mode was chosen.<br>
 * PT_STATUS.PT_INVALID_STOP if an invalid stop bit length was chosen.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_get_rs232_config()
 */
POWERTRACER_API PT_STATUS pt_set_rs232_config(pt_device * powertracer_device, pt_rs232_baudrate baudrate, pt_rs232_parity parity, pt_rs232_stop stop);

/**
 * \ingroup sc_com
 * @brief Obtain configured parameters of EXT_SERIAL channel.
 * 
 * <b>NOTE:</b> This function does not affect SMARTCARD communication channel.\n
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param baudrate A pointer to the pt_rs232_baudrate enumeration structure. Possible values are:<br>
 * <b>BAUD9600</b>, for 9600 bps baudrate,<br>
 * <b>BAUD19200</b>, for 19200 bps baudrate,<br>
 * <b>BAUD38400</b>, for 38400 bps baudrate,<br>
 * <b>BAUD57600</b>, for 57600 bps baudrate,<br>
 * <b>BAUD115200</b>, for 115200 bps baudrate,<br>
 * @param parity A pointer to the pt_rs232_parity enumeration structure. Possible values are:<br>
 * <b>NONE</b>, if parity bit will not be included,<br>
 * <b>ODD</b>, if the odd-parity bit will be included,<br>
 * <b>EVEN</b>, if the even-parity bit will be included.<br>
 * @param stop A pointer to the pt_rs232_stop enumeration structure Possible values are:<br>
 * <b>ONE_STOP</b>, if one stop bit is attached to the end of a character frame,<br>
 * <b>TWO_STOP</b>, if two stop bits are attached to the end of a character frame.<br>
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_set_rs232_config()
 */
POWERTRACER_API PT_STATUS pt_get_rs232_config(pt_device * powertracer_device, pt_rs232_baudrate * baudrate, pt_rs232_parity * parity, pt_rs232_stop * stop);

/**
 * \ingroup sc_ctrl
 * @brief Get maximum and minimum smart card frequency supported by Power Tracer.
 *
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param min Pointer to a double variable where the minimum frequency in MHz is returned.
 * @param max Pointer to a double variable where the maximum frequency in MHz is returned.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * @see pt_set_smartcard_frequency()
 * @see pt_get_smartcard_frequency()
 * @see pt_get_smartcard_frequency_stepsize()
 */
POWERTRACER_API PT_STATUS pt_get_smartcard_frequency_boundary(pt_device * powertracer_device, double * min, double * max);

/**
 * \ingroup sc_ctrl
 * @brief Get the resolution of between the maximum and minimum smart card frequency supported by Power Tracer.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param step_size Pointer to a double variable where the resolution of frequency in MHz is returned.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * @see pt_set_smartcard_frequency()
 * @see pt_get_smartcard_frequency()
 * @see pt_get_smartcard_frequency_boundary()
 */
POWERTRACER_API PT_STATUS pt_get_smartcard_frequency_stepsize(pt_device * powertracer_device, double * step_size);

/**
 * \ingroup misc
 * @brief Fetch the Power Tracer bitstream build time as 32-bit integer representing seconds since the epoch (Jan. 1st, 01:00:00, 1970).
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @build_time A pointer to a 32-bit integer where the time stamp will be stored. 
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_get_hw_build_id()
 */
POWERTRACER_API PT_STATUS pt_get_hw_build_time(pt_device * powertracer_device, unsigned int * build_time);

/**
 * \ingroup misc
 * @brief Fetch the Power Tracer bitstream build ID in the form of ASCII string.
 * @param powertracer_device A pointer to the Power Tracer data structure that stores the device information.
 * @param id_buffer A pointer to a byte array where the build ID will be stored.
 * @param buf_len The size of byte array. Minimum size of 14 bytes is required.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_INVALID_HANDLE if an invalid handle was passed to the function.<br>
 * PT_STATUS.PT_NOT_FOUND if the Power Tracer was not found.<br>
 * PT_STATUS.PT_IO_ERROR if a communication error happened.<br>
 * PT_STATUS.PT_NOT_COMPATIBLE if the Power Tracer in use does not support this function.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_get_hw_build_time()
 */
POWERTRACER_API PT_STATUS pt_get_hw_build_id(pt_device * powertracer_device, unsigned char * id_buffer, unsigned int buf_len);

/**
 * \ingroup misc
 * @brief Undocumented test function for Riscure internal use only.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 */
POWERTRACER_API PT_STATUS pt_test(pt_device * powertracer_device, void * data);

/**
 * \ingroup misc
 * @brief Obtain the number of firmware binaries cached in SDK library.
 * @param n_cached_fw A pointer to an integer value where the firmware count is stored.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_sdk_get_cached_firmware_build_time()
 */
POWERTRACER_API PT_STATUS pt_sdk_get_cached_firmware_count(unsigned int * n_cached_fw);

/**
 * \ingroup misc
 * @brief Obtain the build time of the indexed firmware cache entry, together with its version.
 * @param cache_index The index to the firmware cache entries, index valid from 0 to (n_cached_fw-1).
 * @param version A pointer to pt_version structure, where version information is stored.
 * @param build_time A pointer to an integer where a 32-bit time stamp is stored.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_sdk_get_cached_firmware_count()
 * @see pt_sdk_get_cached_firmware_build_id()
 */
POWERTRACER_API PT_STATUS pt_sdk_get_cached_firmware_build_time(unsigned int cache_index, pt_version * version, unsigned int * build_time);

/**
 * \ingroup misc
 * @brief Obtain the build if of the indexed firmware cache entry, together with its version.
 * @param cache_index The index to the firmware cache entries, index valid from 0 to (n_cached_fw-1).
 * @param version A pointer to pt_version structure, where version information is stored.
 * @param buf_id A pointer to a bytearray where the build id is stored.
 * @param len_buf The size of the bytearray.
 * @warning Not thread safe !
 * @return
 * PT_STATUS.PT_OK if the action was performed successfully.<br>
 * PT_STATUS.PT_FAILED if an error occured during the action.<br>
 * @see pt_sdk_get_cached_firmware_count()
 * @see pt_sdk_get_cached_firmware_build_time()
 * 
 */
POWERTRACER_API PT_STATUS pt_sdk_get_cached_firmware_build_id(unsigned int cache_index, pt_version * version, unsigned char * buf_id, unsigned int len_buf );

/**
 * \ingroup misc
 * @brief Obtain the SDK build time as ASCII string with the format of yyyy-MM-dd-HH:mm:ss.
 * @warning Not thread safe !
 * @return
 * SDK build timestamp.<br>
 * @see pt_sdk_get_version()
 */
POWERTRACER_API char* pt_sdk_get_build_time();
/**
 * \ingroup misc
 * @brief Get the SDK version number.
 * @warning Not thread safe !
 * @return 
 * SDK version number.<br>
 * @see pt_sdk_get_build_time()
 */
POWERTRACER_API char* pt_sdk_get_version();

/**
 * \ingroup misc
 * @brief Checks if this version of the SDK is a snapshot (development) version. Make sure to always use a final version.
 * @warning Not thread safe !
 * @return
 * TRUE if the SDK is a snapshot (development) version.<br>
 * FALSE if the SDK is a final version.<br>
 */
POWERTRACER_API BOOL pt_sdk_is_snapshot_version();

#endif
