/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.crypto_wallet.fragments;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.crypto_wallet.adapters.WalletCoinAdapter;
import org.chromium.chrome.browser.crypto_wallet.listeners.OnWalletListItemClick;
import org.chromium.chrome.browser.crypto_wallet.model.WalletListItemModel;
import org.chromium.chrome.browser.crypto_wallet.util.SmoothLineChartEquallySpaced;
import org.chromium.chrome.browser.crypto_wallet.util.Utils;

import java.util.ArrayList;
import java.util.List;

public class PortfolioFragment extends Fragment implements OnWalletListItemClick {
    public static PortfolioFragment newInstance() {
        return new PortfolioFragment();
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setHasOptionsMenu(true);
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container,
            @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_portfolio, container, false);

        view.setOnTouchListener(new View.OnTouchListener() {
            @Override
            @SuppressLint("ClickableViewAccessibility")
            public boolean onTouch(View v, MotionEvent event) {
                SmoothLineChartEquallySpaced chartES = view.findViewById(R.id.line_chart);
                if (chartES == null) {
                    return true;
                }
                if (event.getAction() == MotionEvent.ACTION_MOVE
                        || event.getAction() == MotionEvent.ACTION_DOWN) {
                    chartES.drawLine(event.getRawX(), null);
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    chartES.drawLine(-1, null);
                }

                return true;
            }
        });

        return view;
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        assert getActivity() != null;

        SmoothLineChartEquallySpaced chartES = view.findViewById(R.id.line_chart);
        chartES.setColors(new int[] {0xFFF73A1C, 0xFFBF14A2, 0xFF6F4CD2});
        chartES.setData(new float[] {15, 21, 9, 21, 25, 35, 24, 28});

        setUpCoinList(view);
        Button editVisibleAssets = view.findViewById(R.id.edit_visible_assets);
        editVisibleAssets.setOnClickListener(v -> {
            EditVisibleAssetsBottomSheetDialogFragment bottomSheetDialogFragment =
                    EditVisibleAssetsBottomSheetDialogFragment.newInstance();
            bottomSheetDialogFragment.show(
                    getFragmentManager(), EditVisibleAssetsBottomSheetDialogFragment.TAG_FRAGMENT);
        });
    }

    private void setUpCoinList(View view) {
        RecyclerView rvCoins = view.findViewById(R.id.rvCoins);
        WalletCoinAdapter walletCoinAdapter =
                new WalletCoinAdapter(WalletCoinAdapter.AdapterType.VISIBLE_ASSETS_LIST);
        List<WalletListItemModel> walletListItemModelList = new ArrayList<>();
        walletListItemModelList.add(new WalletListItemModel(
                R.drawable.ic_eth, "Ethereum", "ETH", "$872.48", "0.31178 ETH"));
        walletCoinAdapter.setWalletListItemModelList(walletListItemModelList);
        walletCoinAdapter.setOnWalletListItemClick(PortfolioFragment.this);
        walletCoinAdapter.setWalletListItemType(Utils.ASSET_ITEM);
        rvCoins.setAdapter(walletCoinAdapter);
        rvCoins.setLayoutManager(new LinearLayoutManager(getActivity()));
    }

    @Override
    public void onAssetClick() {
        Utils.openAssetDetailsActivity(getActivity());
    }
}
