/*###############################################################################
#
# This software is distributed under the following ISC license:
#
# Copyright © 2017 BRAZILIAN SYNCHROTRON LIGHT SOURCE <sol@lnls.br>
#   Dawood Alnajjar <dawood.alnajjar@lnls.br>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
# OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
# CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
# Description:
# Configuration file parser header
#
###############################################################################*/

#ifndef CFG_PARSER_H
#define CFG_PARSER_H

#include <boost/property_tree/ini_parser.hpp>
#include <boost/bimap.hpp>
#include "SCALER.h"
#include "ANALOG.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef boost::bimap< unsigned, std::string > bim_type;
typedef boost::bimap< std::string, unsigned > bm_address_type;

#define BIADDR_ALIAS     "BIAddresses"
#define BO_ALIAS         "BO"
#define AO_ALIAS         "AO"
#define AI_ALIAS         "AI"
#define SCALER_ALIAS     "SCALERS"
#define WAVEFORM_ALIAS   "WAVEFORMS"
#define MBBO_ALIAS       "MBBO"
#define MBBI_ALIAS       "MBBI"
#define BI_VECTOR        "BI_VECTOR"
#define STRINGOUT_ALIAS  "STRINGOUT"
#define STRINGIN_ALIAS   "STRINGIN"

/* This class processes an ini file, and imports it to a
 * boost property tree. It uses the formerly defined aliases
 * to look for keys in the file.
 *
 */
class cfg_parser {
    public:
        cfg_parser();
        cfg_parser(std::string file);
        ~cfg_parser();

        /* Gets settings key from the ini file */
        int get_settings(std::string &ip, std::string &path, std::string &fileName, std::string &signature, bool &use_shared_memory, std::string &shared_memory_path, uint32_t &shared_memory_size);

        /* fills the binary input BI key maps */
        int get_bi_maps(bool rt_support, uint16_t &count, bim_type *bi, bm_address_type *bi_address_map, bm_address_type *bi_rt_address_map);

        /* Fills all indicated address binary maps */
        int get_address_maps(bool rt_support, uint16_t & count, uint16_t &fxp_count, struct fxp_ctx * fxp_ctx, bm_address_type * address_map, bm_address_type * rt_address_map, string alias);

        /* Fills all indicated address scaler maps and structures */
        int get_scaler_data(bm_address_type * scaler_name_index_map, struct scaler_ctx * scaler_ctx);

        /* Fills all waveform data */
        int get_waveform_data(bool rt_support, uint16_t &count, bm_address_type * waveform_name_index_map, bm_address_type *rt_address_map, struct waveform_ctx * waveform_ctx);
    private:
        std::string cfg_file;
        boost::property_tree::ptree tree;
};


#ifdef __cplusplus
}
#endif

#endif
