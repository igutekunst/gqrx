/* -*- c++ -*- */
/*
 * Copyright 2011-2012 Alexandru Csete OZ9AEC.
 * Copyright 2012 Hoernchen
 * Copyright 2012 Mathis Schmieder <mathis.schmieder@googlemail.com>
 *
 * Gqrx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Gqrx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Gqrx; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
#include <iostream>
#include "rx_source_osmosdr.h"


rx_source_osmosdr_sptr make_rx_source_osmosdr(const std::string device_name)
{
    return gnuradio::get_initial_sptr(new rx_source_osmosdr(device_name));
}


rx_source_osmosdr::rx_source_osmosdr(const std::string device_name)
    : rx_source_base("rx_source_osmosdr"),
      d_freq(144.5e6),
      d_gain(20.0)
{
    d_osmosdr_src = osmosdr_make_source_c("");
    /** TODO: check error */

    // populate supported sample rates
    d_sample_rates.push_back(1920000.0);

    connect(d_osmosdr_src, 0, self(), 0);

    /* set_freq(144.5e6f);
    set_gain(20.0f); */
}


rx_source_osmosdr::~rx_source_osmosdr()
{

}


void rx_source_osmosdr::select_device(const std::string device_name)
{
    /** TODO osmo sdr source supports more than one device **/
}

void rx_source_osmosdr::set_freq(double freq)
{
    if ((freq >= get_freq_min()) && (freq <= get_freq_max()))
    {
        d_freq = freq;
        d_osmosdr_src->set_center_freq((float) d_freq); //TODO check if this worked
    }

#ifndef QT_NO_DEBUG_OUTPUT
    std::cout << "OsmoSDR source new freq: " << (int) d_freq << " Hz" << std::endl;
#endif
}

double rx_source_osmosdr::get_freq()
{
    return d_freq;
}

double rx_source_osmosdr::get_freq_min()
{
    return 50.0e6;
}

double rx_source_osmosdr::get_freq_max()
{
    return 2.0e9;
}

void rx_source_osmosdr::set_gain(double gain)
{
    unsigned char g;

    if ((gain >= get_gain_min()) && (gain <= get_gain_max()))
    {
        d_gain = gain;

        /* convert gain to nearest discrete value */
        if (gain > 27.5)
            g = 14;              // 30.0 dB
        else if (gain > 22.5)
            g = 13;              // 25.0 dB
        else if (gain > 18.75)
            g = 12;              // 20.0 dB
        else if (gain > 16.25)
            g = 11;              // 17.5 dB
        else if (gain > 13.75)
            g = 10;              // 15.0 dB
        else if (gain > 11.25)
            g = 9;               // 12.5 dB
        else if (gain > 8.75)
            g = 8;               // 10.0 dB
        else if (gain > 6.25)
            g = 7;               // 7.5 dB
        else if (gain > 3.75)
            g = 6;               // 5.0 dB
        else if (gain > 1.25)
            g = 5;               // 2.5 dB
        else if (gain > -1.25)
            g = 4;               // 0.0 dB
        else if (gain > -3.75)
            g = 1;               // -2.5 dB
        else
            g = 0;               // -5.0 dB

	// TODO set auto gain / set manual gain
    }
}

double rx_source_osmosdr::get_gain()
{
    return d_gain;
}

double rx_source_osmosdr::get_gain_min()
{
    return -5.0;
}

double rx_source_osmosdr::get_gain_max()
{
    return 30.0;
}

void rx_source_osmosdr::set_sample_rate(double sps)
{
    /** TODO rtl has more than one possible sample rate... */
    // nothing to do;
}

double rx_source_osmosdr::get_sample_rate()
{
    return 1920000.0;
}

std::vector<double> rx_source_osmosdr::get_sample_rates()
{
    return d_sample_rates;
}
