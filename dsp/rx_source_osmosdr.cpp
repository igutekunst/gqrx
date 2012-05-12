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
      d_freq(144.5e6), // TODO maybe not hardcode this
      d_gain(20.0),
      d_sample_rate(1920000.0)
{
    d_osmosdr_src = osmosdr_make_source_c("");
    /** TODO: check error */

    // populate supported sample rates
    d_sample_rates.push_back(d_sample_rate);

    connect(d_osmosdr_src, 0, self(), 0);

    // setting initial frequency and sample rate
    set_freq(d_freq);
    set_sample_rate(d_sample_rate);
    d_osmosdr_src->set_gain_mode(false); // manual gain
    set_gain(d_gain);
}


rx_source_osmosdr::~rx_source_osmosdr()
{
    /** TODO nothing to do here? */
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
    return d_osmosdr_src->get_center_freq();
}

double rx_source_osmosdr::get_freq_min()
{
    return d_osmosdr_src->get_freq_range()[0].start();
}

double rx_source_osmosdr::get_freq_max()
{
    return d_osmosdr_src->get_freq_range()[0].stop();
}

void rx_source_osmosdr::set_gain(double gain)
{
    if ((gain >= get_gain_min()) && (gain <= get_gain_max()))
    {
        d_gain = gain;
        d_osmosdr_src->set_gain(gain);
    }
}

double rx_source_osmosdr::get_gain()
{
    return d_osmosdr_src->get_gain();
}

void rx_source_osmosdr::set_gain_mode(int gain_mode)
{
    d_osmosdr_src->set_gain_mode(gain_mode);
}

double rx_source_osmosdr::get_gain_min()
{
    return d_osmosdr_src->get_gain_range().begin()->start();
}

double rx_source_osmosdr::get_gain_max()
{
    return d_osmosdr_src->get_gain_range().rbegin()->stop();
}

void rx_source_osmosdr::set_sample_rate(double sps)
{
    d_osmosdr_src->set_sample_rate(sps);
}

double rx_source_osmosdr::get_sample_rate()
{
    return d_osmosdr_src->get_sample_rate();
}

std::vector<double> rx_source_osmosdr::get_sample_rates()
{
    return d_sample_rates;
}

void rx_source_osmosdr::set_freq_corr(int ppm)
{
    d_osmosdr_src->set_freq_corr(ppm);
    set_freq(d_freq);
}
